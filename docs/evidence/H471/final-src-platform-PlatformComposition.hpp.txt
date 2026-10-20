#pragma once

#include <ESPressio_System.hpp>

namespace ESPressio::Platform {

    namespace Framework = ESPressio::System::CompositionFramework;

    /// Composition domain containing application-selected Platform capabilities.
    struct Domain final : Framework::Domain {};

} // ESPressio::Platform
