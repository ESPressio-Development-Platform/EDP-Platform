#include <cstdint>

#include <ESPressio_Platform.hpp>
#include <ESPressio_Platform_Portable_AtomicWord32.hpp>

namespace Demo {

    /// Small state published coherently through ConcurrentSnapshot.
    struct SharedState final {

        // Published values.

        /// Demonstration generation number.
        std::uint32_t Generation;

        /// Value associated with the generation.
        std::uint32_t Value;

    };


    /// Runs the ConcurrentSnapshot demonstration.
    int Run() noexcept {
        using AtomicProvider = ESPressio::Platform::Portable::Concurrency::AtomicWord32Provider;
        using StrongTraits =
            ESPressio::Platform::Concurrency::Detail::SequentiallyConsistentAtomicWord32ProviderTraits<AtomicProvider>;
        static_assert(sizeof(StrongTraits) > 0U);

        AtomicProvider::Word publication;
        publication.StoreSequentiallyConsistent(1U);
        if (publication.LoadSequentiallyConsistent() != 1U) { return 2; }

        using Snapshot = ESPressio::Platform::Concurrency::ConcurrentSnapshot<
            SharedState,
            AtomicProvider
        >;

        Snapshot snapshot(
            SharedState {
                0U,
                100U
            }
        );

        snapshot.Publish(
            SharedState {
                1U,
                200U
            }
        );

        const auto current = snapshot.Read();

        return
            current.Generation == 1U &&
            current.Value == 200U
                ? 0
                : 1;
    }

} // Demo

/// Runs the demonstration from the ESP-IDF application entry point.
extern "C" void app_main() {
    static_cast<void>(
        Demo::Run()
    );
}
