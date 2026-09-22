#include <cstddef>
#include <cstdint>

#include <ESPressio_Platform.hpp>

namespace Demo {

    namespace Framework = ESPressio::System::CompositionFramework;

    /// Minimal example provider demonstrating the Platform random-byte contract.
    class ExampleRandomProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Provides<
            Framework::Offer<
                ESPressio::Platform::Randomness::RandomByteSource,
                Framework::FlagPropertyValue<
                    ESPressio::Platform::Randomness::RandomByteSourceFeatures,
                    ESPressio::Platform::Randomness::RandomByteSourceFeature::CryptographicallySuitable
                >
            >
        >
    > {

        private:

            // Example deterministic state.

            /// Small evolving state used only to make the demo visibly populate bytes.
            std::uint32_t _state = 0x9E3779B9U;

        public:

            // Random-byte generation.

            /// Fills caller-owned storage.
            ESPressio::Platform::Randomness::RandomByteGenerationResult FillRandomBytes(
                std::uint8_t* destination,
                std::size_t byteCount
            ) noexcept {
                if (byteCount == 0U) {
                    return ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded;
                }

                if (destination == nullptr) {
                    return ESPressio::Platform::Randomness::RandomByteGenerationResult::InvalidDestination;
                }

                for (std::size_t index = 0U; index < byteCount; ++index) {
                    _state ^= _state << 13U;
                    _state ^= _state >> 17U;
                    _state ^= _state << 5U;
                    destination[index] = static_cast<std::uint8_t>(_state);
                }

                return ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded;
            }

    };


    /// Exercises the random-byte contract with caller-owned storage.
    int Run() noexcept {
        ExampleRandomProvider provider;
        std::uint8_t bytes[16U]{};

        return provider.FillRandomBytes(
            bytes,
            sizeof(bytes)
        ) == ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded
            ? 0
            : 1;
    }

} // Demo


/// Runs the random-byte capability demonstration.
extern "C" void app_main() {
    static_cast<void>(
        Demo::Run()
    );
}
