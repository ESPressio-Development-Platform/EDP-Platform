#include <cassert>
#include <cstddef>
#include <cstdint>

#include <ESPressio_Platform.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;

    /// Deterministic test provider used only to exercise the RandomByteSource contract.
    class TestRandomProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Randomness::RandomByteSource,
                Framework::FlagPropertyValue<
                    ESPressio::Platform::Randomness::RandomByteSourceFeatures,
                    ESPressio::Platform::Randomness::RandomByteSourceFeature::CryptographicallySuitable
                >
            >
        >
    > {

        public:

            // Random-byte generation.

            /// Fills caller-owned storage with deterministic bytes for contract testing.
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
                    destination[index] = static_cast<std::uint8_t>(index + 1U);
                }

                return ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded;
            }

    };


    /// Exercises RandomByteSource contract metadata and operation semantics.
    int Run() noexcept {
        using Contract = ESPressio::Platform::Randomness::Detail::RandomByteSourceProviderTraits<TestRandomProvider>;
        using Properties = typename Contract::Properties;

        static_assert(
            Properties::template Contains<ESPressio::Platform::Randomness::RandomByteSourceFeatures>,
            "Random provider must expose its feature flags"
        );

        constexpr ESPressio::System::FlagSet<
            ESPressio::Platform::Randomness::RandomByteSourceFeature,
            std::uint8_t
        > features(
            Properties::template Value<ESPressio::Platform::Randomness::RandomByteSourceFeatures>
        );

        static_assert(
            features.HasAll(
                ESPressio::Platform::Randomness::RandomByteSourceFeature::CryptographicallySuitable
            ),
            "Test provider must advertise cryptographic suitability"
        );

        TestRandomProvider provider;
        std::uint8_t bytes[4U]{};

        assert(
            provider.FillRandomBytes(
                bytes,
                sizeof(bytes)
            ) == ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded
        );

        assert(bytes[0U] == 1U);
        assert(bytes[3U] == 4U);

        assert(
            provider.FillRandomBytes(
                nullptr,
                1U
            ) == ESPressio::Platform::Randomness::RandomByteGenerationResult::InvalidDestination
        );

        assert(
            provider.FillRandomBytes(
                nullptr,
                0U
            ) == ESPressio::Platform::Randomness::RandomByteGenerationResult::Succeeded
        );

        return 0;
    }

} // Test


/// Runs the Platform randomness contract tests.
int main() {
    return Test::Run();
}
