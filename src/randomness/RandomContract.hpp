#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "RandomComposition.hpp"
#include "RandomTypes.hpp"

namespace ESPressio::Platform::Randomness::Detail {

    /// Validates the public contract required from a RandomByteSource provider.
    ///
    /// @tparam TRandomByteSourceProvider Concrete provider type being validated.
    template<class TRandomByteSourceProvider>
    struct RandomByteSourceProviderTraits {

        static_assert(
            TRandomByteSourceProvider::CompositionCapabilities::template Contains<RandomByteSource>,
            "RandomByteSource provider must supply the RandomByteSource capability"
        );

        // Advertised capability metadata.

        /// Properties advertised for RandomByteSource.
        using Properties = typename TRandomByteSourceProvider::CompositionCapabilities::template PropertiesFor<RandomByteSource>;

        static_assert(
            Properties::template Contains<RandomByteSourceFeatures>,
            "RandomByteSource provider must advertise RandomByteSourceFeatures"
        );


        // Operation return types.

        /// Return type produced by random-byte generation.
        using FillResult = decltype(
            std::declval<TRandomByteSourceProvider&>().FillRandomBytes(
                std::declval<std::uint8_t*>(),
                std::declval<std::size_t>()
            )
        );

        static_assert(
            std::is_same_v<FillResult, RandomByteGenerationResult>,
            "RandomByteSource FillRandomBytes must return RandomByteGenerationResult"
        );

        static_assert(
            noexcept(
                std::declval<TRandomByteSourceProvider&>().FillRandomBytes(
                    std::declval<std::uint8_t*>(),
                    std::declval<std::size_t>()
                )
            ),
            "RandomByteSource FillRandomBytes must be noexcept"
        );

    };

} // ESPressio::Platform::Randomness::Detail
