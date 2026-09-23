#include <array>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <thread>
#include <type_traits>
#include <vector>

#include <ESPressio_Platform.hpp>

namespace ESPressio::Platform::Concurrency::Tests {

    /// Host-test provider satisfying the AtomicWord32 Platform capability.
    class TestAtomicWord32Provider final : public Framework::Provider<
        Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                AtomicWord32,
                Framework::PropertyValue<LockFree, true>,
                Framework::PropertyValue<AtomicWordStorageBytes, 4U>
            >
        >
    > {
    public:

        /// Four-byte lock-free atomic word used only by the host validation suite.
        class Word final {
        private:

            // Atomic storage.

            /// Host atomic value used to exercise the abstraction contract.
            std::atomic<std::uint32_t> _value;

        public:

            // Construction and lifetime.

            /// Creates a test word initialized to zero.
            constexpr Word() noexcept :
                _value(0U) {}

            /// Prevents copying synchronization storage.
            Word(const Word&) = delete;

            /// Prevents copy assignment of synchronization storage.
            Word& operator =(const Word&) = delete;

            /// Prevents relocation of synchronization storage.
            Word(Word&&) = delete;

            /// Prevents move assignment of synchronization storage.
            Word& operator =(Word&&) = delete;


            // Atomic loads.

            /// Reads the value with relaxed ordering.
            std::uint32_t LoadRelaxed() const noexcept {
                return _value.load(
                    std::memory_order_relaxed
                );
            }

            /// Reads the value with acquire ordering.
            std::uint32_t LoadAcquire() const noexcept {
                return _value.load(
                    std::memory_order_acquire
                );
            }


            // Atomic stores.

            /// Replaces the value with relaxed ordering.
            void StoreRelaxed(
                std::uint32_t value
            ) noexcept {
                _value.store(
                    value,
                    std::memory_order_relaxed
                );
            }

            /// Replaces the value with release ordering.
            void StoreRelease(
                std::uint32_t value
            ) noexcept {
                _value.store(
                    value,
                    std::memory_order_release
                );
            }


            /// Replaces the word when it matches the expected value using acquire/release ordering.
            bool CompareExchangeAcqRel(
                std::uint32_t& expected,
                std::uint32_t desired
            ) noexcept {
                return _value.compare_exchange_strong(
                    expected,
                    desired,
                    std::memory_order_acq_rel,
                    std::memory_order_acquire
                );
            }

        };

    };


    static_assert(
        std::atomic<std::uint32_t>::is_always_lock_free,
        "Host validation requires lock-free std::atomic<std::uint32_t>"
    );

    static_assert(
        sizeof(TestAtomicWord32Provider::Word) == sizeof(std::uint32_t),
        "Host validation requires a four-byte atomic word"
    );


    /// Correlated state used to detect torn snapshot reads.
    struct SnapshotState final {

        // Correlated test values.

        /// Publication sequence used as the source for every other field.
        std::uint32_t Sequence;

        /// Bitwise inverse of Sequence.
        std::uint32_t Inverse;

        /// Sequence promoted and multiplied by two.
        std::uint64_t Doubled;

        /// Fixed xor relationship used to detect torn reads.
        std::uint64_t Check;

    };


    /// Builds one internally correlated state used to detect torn snapshot reads.
    SnapshotState MakeState(
        std::uint32_t sequence
    ) noexcept {
        return SnapshotState {
            sequence,
            static_cast<std::uint32_t>(~sequence),
            static_cast<std::uint64_t>(sequence) * 2ULL,
            static_cast<std::uint64_t>(sequence) ^ 0xA5A55A5AF0F00F0FULL
        };
    }


    /// Determines whether every field belongs to the same published test generation.
    bool IsCoherent(
        const SnapshotState& state
    ) noexcept {
        return
            (state.Inverse == static_cast<std::uint32_t>(~state.Sequence)) &&
            (state.Doubled == (static_cast<std::uint64_t>(state.Sequence) * 2ULL)) &&
            (state.Check == (static_cast<std::uint64_t>(state.Sequence) ^ 0xA5A55A5AF0F00F0FULL));
    }

} // ESPressio::Platform::Concurrency::Tests


int main() {
    using namespace ESPressio::Platform::Concurrency;
    using namespace ESPressio::Platform::Concurrency::Tests;

    using Provider = TestAtomicWord32Provider;
    using Properties = typename Provider::CompositionOffers::template PropertiesFor<AtomicWord32>;

    static_assert(Properties::template Contains<LockFree>);
    static_assert(Properties::template Value<LockFree>);
    static_assert(Properties::template Value<AtomicWordStorageBytes> == 4U);

    using CompactState = std::array<std::uint8_t, 5U>;

    ConcurrentSnapshot<CompactState, Provider> compact(
        CompactState {
            1U,
            2U,
            3U,
            4U,
            5U
        }
    );

    const auto compactRead = compact.Read();
    assert(compactRead[0U] == 1U);
    assert(compactRead[4U] == 5U);
    static_assert(ConcurrentSnapshot<CompactState, Provider>::StateBytes == 5U);
    static_assert(ConcurrentSnapshot<CompactState, Provider>::WordsPerBuffer == 2U);

    ConcurrentSnapshot<SnapshotState, Provider> snapshot(MakeState(0U));
    assert(IsCoherent(snapshot.Read()));

    snapshot.Publish(MakeState(1U));
    const auto first = snapshot.Read();
    assert(first.Sequence == 1U);
    assert(IsCoherent(first));

    static_assert(ConcurrentSnapshot<SnapshotState, Provider>::StateBytes == sizeof(SnapshotState));
    static_assert(ConcurrentSnapshot<SnapshotState, Provider>::AtomicStorageBytes == 56U);

    constexpr std::uint32_t publicationCount = 100000U;
    constexpr std::size_t readerCount = 8U;

    std::atomic<bool> stop { false };
    std::atomic<bool> failed { false };
    std::vector<std::thread> readers;
    readers.reserve(readerCount);

    for (std::size_t readerIndex = 0U; readerIndex < readerCount; ++readerIndex)
        readers.emplace_back([&snapshot, &stop, &failed]() {
            while (!stop.load(std::memory_order_acquire)) {
                const auto state = snapshot.Read();

                if (!IsCoherent(state)) {
                    failed.store(
                        true,
                        std::memory_order_release
                    );

                    return;
                }
            }
        });

    for (std::uint32_t sequence = 2U; sequence <= publicationCount; ++sequence)
        snapshot.Publish(MakeState(sequence));

    stop.store(
        true,
        std::memory_order_release
    );

    for (auto& reader : readers)
        reader.join();

    assert(!failed.load(std::memory_order_acquire));

    const auto finalState = snapshot.Read();
    assert(finalState.Sequence == publicationCount);
    assert(IsCoherent(finalState));

    return 0;
}
