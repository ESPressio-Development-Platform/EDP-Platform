#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include "AtomicWord32Contract.hpp"

namespace ESPressio::Platform::Concurrency {

    namespace Detail {

        /// Coherent publication generation: 31 high epoch bits plus 32 low bits.
        struct SnapshotGeneration final {

            // Generation coordinates.

            /// High 31 bits of the logical publication generation.
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
    /// Data-word acquire/release ordering prevents acceptance of a partially reused buffer.
    /// A full 2^63-publication cycle during one read is outside the finite generation contract.
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

        // The top high-word bit denotes a rollover with the old published buffer readable.
        static constexpr std::uint32_t RolloverMarker = 0x80000000U;
        static constexpr std::uint32_t EpochMask = 0x7FFFFFFFU;
        static constexpr std::uint32_t MaximumLow = 0xFFFFFFFFU;

        // Snapshot storage.

        /// Alternating fixed buffers holding atomic representations of published state.
        std::array<Buffer, 2U> _buffers;

        /// High epoch plus transient rollover marker; no atomic 64-bit operation.
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

        /// Replaces one inactive buffer. Release on EVERY word connects later buffer reuse
        /// to the reader that acquires that word, so its final generation cannot remain stale.
        void WriteBuffer(
            std::size_t bufferIndex,
            const TState& state
        ) noexcept {
            const auto encoded = Encode(state);

            for (std::size_t index = 0U; index < WordCount; ++index)
                _buffers[bufferIndex][index].StoreRelease(
                    encoded[index]
                );
        }

        /// Acquires every copied word, including a word observed from a subsequent reuse.
        /// Such a read imports the writer publication preceding that reuse.
        PlainBuffer ReadBuffer(
            std::size_t bufferIndex
        ) const noexcept {
            PlainBuffer encoded {};

            for (std::size_t index = 0U; index < WordCount; ++index)
                encoded[index] = _buffers[bufferIndex][index].LoadAcquire();

            return encoded;
        }


        // Generation access.

        /// Reads a coherent publication identity using existing 32-bit acquire operations.
        /// A stable rollover marker denotes the OLD published identity, not a wait condition.
        Detail::SnapshotGeneration ReadGeneration() const noexcept {
            for (;;) {
                const auto highBefore = _generationHigh.LoadAcquire();
                const auto low = _generationLow.LoadAcquire();
                const auto highAfter = _generationHigh.LoadAcquire();
                if (highBefore != highAfter) continue;
                if ((highBefore & RolloverMarker) != 0U) {
                    // The sole writer has not completed the new epoch. Buffer1 still holds
                    // the last published MaximumLow state; a paused writer cannot block us.
                    return {highBefore & EpochMask, MaximumLow};
                }
                return {highBefore, low};
            }
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
            const auto targetBuffer = static_cast<std::size_t>(nextLow & 1U);
            WriteBuffer(targetBuffer, state);
            if (nextLow == 0U) {
                // Until the final high release, readers normalize the marker to the previous
                // MaximumLow publication in buffer1. That buffer is not reused in this phase.
                _generationHigh.StoreRelease(currentHigh | RolloverMarker);
                _generationLow.StoreRelease(0U);
                _generationHigh.StoreRelease(static_cast<std::uint32_t>((currentHigh + 1U) & EpochMask));
            } else {
                _generationLow.StoreRelease(nextLow);
            }
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
