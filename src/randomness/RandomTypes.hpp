#pragma once

#include <cstdint>

namespace ESPressio::Platform::Randomness {

    /// Outcome from requesting random bytes from one Platform provider.
    enum class RandomByteGenerationResult : std::uint8_t {
        Succeeded = 0,
        InvalidDestination = 1,
        ProviderFailure = 2
    };

} // ESPressio::Platform::Randomness
