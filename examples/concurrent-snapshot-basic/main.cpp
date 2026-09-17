#include <atomic>
#include <cstdint>

#include <ESPressio_Platform.hpp>

namespace Example {

    namespace Framework = ESPressio::System::CompositionFramework;

    /// Example application provider satisfying the Platform AtomicWord32 capability.
    class AtomicProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Provides<
            Framework::Offer<
                ESPressio::Platform::Concurrency::AtomicWord32,
                Framework::PropertyValue<ESPressio::Platform::Concurrency::LockFree, true>,
                Framework::PropertyValue<ESPressio::Platform::Concurrency::AtomicWordStorageBytes, 4U>
            >
        >
    > {
    public:

        /// Minimal atomic word used by this standalone host example.
        class Word final {
        private:

            // Atomic storage.

            /// Atomic value represented by this word.
            std::atomic<std::uint32_t> _value;

        public:

            // Construction and lifetime.

            /// Creates an atomic word initialized to zero.
            constexpr Word() noexcept :
                _value(0U) {}

            /// Prevents copying atomic storage.
            Word(const Word&) = delete;

            /// Prevents copy assignment of atomic storage.
            Word& operator =(const Word&) = delete;

            /// Prevents relocating atomic storage.
            Word(Word&&) = delete;

            /// Prevents move assignment of atomic storage.
            Word& operator =(Word&&) = delete;


            // Atomic access.

            /// Reads the word with relaxed ordering.
            std::uint32_t LoadRelaxed() const noexcept {
                return _value.load(
                    std::memory_order_relaxed
                );
            }

            /// Reads the word with acquire ordering.
            std::uint32_t LoadAcquire() const noexcept {
                return _value.load(
                    std::memory_order_acquire
                );
            }

            /// Stores the word with relaxed ordering.
            void StoreRelaxed(
                std::uint32_t value
            ) noexcept {
                _value.store(
                    value,
                    std::memory_order_relaxed
                );
            }

            /// Stores the word with release ordering.
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


    /// Small read-mostly state published as one coherent unit.
    struct SharedState final {

        // Published values.

        /// Current generation of the example state.
        std::uint32_t Generation;

        /// Value associated with Generation.
        std::uint32_t Value;

    };

} // Example


int main() {
    using Snapshot = ESPressio::Platform::Concurrency::ConcurrentSnapshot<
        Example::SharedState,
        Example::AtomicProvider
    >;

    Snapshot state(
        Example::SharedState {
            0U,
            100U
        }
    );

    // Exactly one writer publishes complete replacement states. Readers never observe one field
    // from the old state combined with another field from the new state.
    state.Publish(
        Example::SharedState {
            1U,
            200U
        }
    );

    const auto current = state.Read();

    return
        (current.Generation == 1U) &&
        (current.Value == 200U)
            ? 0
            : 1;
}
