#pragma once

#include "../platform/PlatformComposition.hpp"

namespace ESPressio::Platform::Execution {

    /// Exclusive domain-neutral binding of one owner-selected record Type to the current context.
    struct CurrentContextBinding final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Indicates that binding lookup has bounded constant cost independent of registered contexts.
    struct ConstantTimeLookup final : Framework::Property<CurrentContextBinding, bool> {};


    /// Indicates that lookup, including the first unbound lookup, performs no allocation.
    struct AllocationFreeLookup final : Framework::Property<CurrentContextBinding, bool> {};


    /// Indicates that operations inspect or change only the calling ordinary execution context.
    struct CurrentContextOnly final : Framework::Property<CurrentContextBinding, bool> {};


    /// Indicates that interrupt operations reject before exposing an interrupted context's record.
    struct RejectsInterruptContexts final : Framework::Property<CurrentContextBinding, bool> {};

} // ESPressio::Platform::Execution
