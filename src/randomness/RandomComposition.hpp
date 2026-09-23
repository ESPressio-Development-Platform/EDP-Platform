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


    /// Qualification requiring a RandomByteSource suitable for cryptographic use.
    using CryptographicRandomByteSourceConstraint = Framework::HasAllFlags<
        RandomByteSourceFeatures,
        RandomByteSourceFeature::CryptographicallySuitable
    >;

    /// Standalone consumer Requirement selecting cryptographically suitable randomness.
    using CryptographicRandomByteSourceRequirement = Framework::Requirement<
        RandomByteSource,
        Framework::RequirementScope::AnyDomain,
        Framework::AtLeastProviders<1U>,
        CryptographicRandomByteSourceConstraint
    >;

} // ESPressio::Platform::Randomness
