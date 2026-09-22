#pragma once

#include <cstdint>

#include "../platform/PlatformComposition.hpp"

namespace ESPressio::Platform::Randomness {

    /// Shared Platform capability supplying unpredictable bytes.
    struct RandomByteSource final : Framework::SharedCapability<Platform::Domain> {};


    /// Finite features advertised by one RandomByteSource provider.
    enum class RandomByteSourceFeature : std::uint8_t {
        CryptographicallySuitable = 0
    };


    /// Feature set advertised by one RandomByteSource provider.
    struct RandomByteSourceFeatures final : Framework::FlagProperty<
        RandomByteSource,
        RandomByteSourceFeature,
        std::uint8_t
    > {};


    /// Requirement selecting a RandomByteSource suitable for cryptographic use.
    using CryptographicRandomByteSourceNeed = Framework::Need<
        RandomByteSource,
        Framework::HasAllFlags<
            RandomByteSourceFeatures,
            RandomByteSourceFeature::CryptographicallySuitable
        >
    >;

} // ESPressio::Platform::Randomness
