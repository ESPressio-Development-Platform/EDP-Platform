#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "AtomicWord32Contract.hpp"

namespace ESPressio::Platform::Concurrency {

    namespace Detail {

        /// Coherent publication generation represented by two lock-free 32-bit atomic words.
        struct SnapshotGeneration final {

            // Generation coordinates.

            /// High 32 bits of the logical publication generation.
            std::uint32_t High;

            /// Low 32 bits of the logical publication generation.
            std::uint32_t Low;

        };


        /// Determines whether two publication generations identify the same published snapshot.
        constexpr bool SameGeneration(
            const SnapshotGeneration& left,
            const SnapshotGeneration& right
        ) noexcept {
            return
                (left.High == right.High) &&
                (left.Low == right.Low);
        }

    } // ESPressio::Platform::Concurrency::Detail


    /// Fixed-storage single-writer/multi-reader coherent state publication.
    ///
    /// Two atomic-word buffers are retained. The single writer always fills the inactive buffer
    /// before publishing a new generation. Readers copy only from the currently published buffer
    /// and validate that the publication generation did not change while the copy was in progress.
    /// A reader may retry when publication overlaps its read, but it never waits for a writer-held
    /// mutex, critical section, task, semaphore, or other blocking synchronization primitive.
    ///
    /// @tparam TState Trivially-copyable state published as one coherent snapshot.
    /// @tparam TAtomicWordProvider Provider satisfying the AtomicWord32 capability.
    template<class TState, class TAtomicWordProvider>
    class ConcurrentSnapshot final {
    private:

        /// Validated atomic-provider metadata used by this snapshot.
        using AtomicTraits = Detail::AtomicWord32ProviderTraits<TAtomicWordProvider>;

        /// Concrete four-byte atomic word supplied by the selected provider.
        using Word = typename AtomicTraits::Word;

        /// Number of atomic words required to store one complete state representation.
        static constexpr std::size_t WordCount =
            (sizeof(TState) + sizeof(std::uint32_t) - 1U) /
            sizeof(std::uint32_t);

        /// Atomic-word array holding one complete state representation.
        using Buffer = std::array<Word, WordCount>;

        /// Plain-word array used while encoding or decoding one state representation.
        using PlainBuffer = std::array<std::uint32_t, WordCount>;

        static_assert(
            std::is_trivially_copyable_v<TState>,
            "ConcurrentSnapshot state must be trivially copyable"
        );

        static_assert(
            std::is_default_constructible_v<TState>,
            "ConcurrentSnapshot state must be default constructible"
        );

        static_assert(
            WordCount > 0U,
            "ConcurrentSnapshot state must occupy at least one byte"
        );

        // Snapshot storage.

        /// Alternating fixed buffers holding atomic representations of published state.
        std::array<Buffer, 2U> _buffers;

        /// High 32 bits of the logical publication generation.
        Word _generationHigh;

        /// Low 32 bits of the logical publication generation and active-buffer selector.
        Word _generationLow;


        // Representation conversion.

        /// Converts a state object into zero-padded 32-bit words without type-punning.
        static PlainBuffer Encode(
            const TState& state
        ) noexcept {
            PlainBuffer encoded {};

            std::memcpy(
                encoded.data(),
                &state,
                sizeof(TState)
            );

            return encoded;
        }

        /// Reconstructs a state object from its 32-bit word representation without type-punning.
        static TState Decode(
            const PlainBuffer& encoded
        ) noexcept {
            TState state {};

            std::memcpy(
                &state,
                encoded.data(),
                sizeof(TState)
            );

            return state;
        }


        // Buffer access.

        /// Replaces one inactive atomic buffer with the supplied state representation.
        void WriteBuffer(
            std::size_t bufferIndex,
            const TState& state
        ) noexcept {
            const auto encoded = Encode(state);

            for (std::size_t index = 0U; index < WordCount; ++index)
                _buffers[bufferIndex][index].StoreRelaxed(
                    encoded[index]
                );
        }

        /// Copies one published atomic buffer into a plain state representation.
        PlainBuffer ReadBuffer(
            std::size_t bufferIndex
        ) const noexcept {
            PlainBuffer encoded {};

            for (std::size_t index = 0U; index < WordCount; ++index)
                encoded[index] = _buffers[bufferIndex][index].LoadRelaxed();

            return encoded;
        }


        // Generation access.

        /// Reads the logical 64-bit publication generation using only lock-free 32-bit operations.
        Detail::SnapshotGeneration ReadGeneration() const noexcept {
            const auto low = _generationLow.LoadAcquire();
            const auto high = _generationHigh.LoadRelaxed();

            return Detail::SnapshotGeneration {
                high,
                low
            };
        }

    public:

        // Construction and lifetime.

        /// Creates a snapshot whose initial published state is value-initialized.
        ConcurrentSnapshot() noexcept :
            ConcurrentSnapshot(TState {}) {}

        /// Creates a snapshot with the supplied initial state already coherently published.
        ///
        /// @param initialState Initial state visible to every reader before the first Publish call.
        explicit ConcurrentSnapshot(
            const TState& initialState
        ) noexcept :
            _buffers {},
            _generationHigh {},
            _generationLow {} {
            _generationHigh.StoreRelaxed(0U);
            _generationLow.StoreRelaxed(0U);

            WriteBuffer(
                0U,
                initialState
            );

            WriteBuffer(
                1U,
                initialState
            );
        }

        /// Prevents duplicating live concurrency state.
        ConcurrentSnapshot(const ConcurrentSnapshot&) = delete;

        /// Prevents copy assignment of live concurrency state.
        ConcurrentSnapshot& operator =(const ConcurrentSnapshot&) = delete;

        /// Prevents relocating live concurrency state while readers may reference it.
        ConcurrentSnapshot(ConcurrentSnapshot&&) = delete;

        /// Prevents move assignment of live concurrency state.
        ConcurrentSnapshot& operator =(ConcurrentSnapshot&&) = delete;


        // Memory metadata.

        /// Number of bytes in the consumer state represented by this snapshot.
        static constexpr std::size_t StateBytes = sizeof(TState);

        /// Number of atomic words used by one state buffer.
        static constexpr std::size_t WordsPerBuffer = WordCount;

        /// Number of bytes reserved for the two state buffers and logical publication generation.
        static constexpr std::size_t AtomicStorageBytes =
            ((2U * WordCount) + 2U) * sizeof(std::uint32_t);


        // Snapshot publication.

        /// Publishes a complete replacement state for concurrent readers.
        ///
        /// Exactly one execution context may call Publish for a given ConcurrentSnapshot object.
        /// The single-writer contract allows publication to remain lock-free without retaining a
        /// writer mutex or compare/exchange ownership word.
        ///
        /// @param state Complete replacement state to publish.
        void Publish(
            const TState& state
        ) noexcept {
            const auto currentLow = _generationLow.LoadRelaxed();
            const auto currentHigh = _generationHigh.LoadRelaxed();
            const auto nextLow = static_cast<std::uint32_t>(currentLow + 1U);
            const auto nextHigh = static_cast<std::uint32_t>(
                currentHigh +
                (nextLow == 0U ? 1U : 0U)
            );
            const auto targetBuffer = static_cast<std::size_t>(nextLow & 1U);

            // The target buffer is inactive under the currently published generation. Populate it
            // completely before changing the generation observed by readers.
            WriteBuffer(
                targetBuffer,
                state
            );

            // On low-word rollover, publish the new high word first. The subsequent release store
            // of the low word makes both the high word and the completed buffer visible together.
            if (nextLow == 0U) _generationHigh.StoreRelaxed(nextHigh);

            _generationLow.StoreRelease(nextLow);
        }


        // Snapshot reading.

        /// Returns one coherent state snapshot to a concurrent reader.
        ///
        /// A publication overlapping the copy causes this method to retry from the newly published
        /// buffer. Reader execution never depends on the writer being scheduled or completing a
        /// critical section, so pre-empting a writer cannot deadlock a reader.
        TState Read() const noexcept {
            for (;;) {
                const auto before = ReadGeneration();
                const auto activeBuffer = static_cast<std::size_t>(before.Low & 1U);
                const auto encoded = ReadBuffer(activeBuffer);
                const auto after = ReadGeneration();

                if (Detail::SameGeneration(
                    before,
                    after
                )) {
                    return Decode(encoded);
                }
            }
        }

    };

} // ESPressio::Platform::Concurrency
