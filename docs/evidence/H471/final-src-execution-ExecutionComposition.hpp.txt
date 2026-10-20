#pragma once

#include <cstddef>
#include <cstdint>

#include "../platform/PlatformComposition.hpp"

namespace ESPressio::Platform::Execution {

    /// Exclusive Platform capability supplying one native schedulable execution-context implementation.
    struct ExecutionContext final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Indicates whether an ExecutionContext provider uses only caller-supplied task/control storage.
    struct CallerSuppliedStorage final : Framework::Property<ExecutionContext, bool> {};


    /// Indicates whether an ExecutionContext provider supports explicit priority configuration.
    struct SupportsPriority final : Framework::Property<ExecutionContext, bool> {};


    /// Indicates whether an ExecutionContext provider supports processor-affinity requests.
    struct SupportsProcessorAffinity final : Framework::Property<ExecutionContext, bool> {};


    /// Indicates whether an ExecutionContext provider can report minimum remaining stack bytes.
    struct SupportsStackTelemetry final : Framework::Property<ExecutionContext, bool> {};


    /// Number of caller-supplied bytes required for native execution-control storage.
    struct ControlStorageBytes final : Framework::Property<ExecutionContext, std::size_t> {};


    /// Required alignment of caller-supplied native execution-control storage.
    struct ControlStorageAlignment final : Framework::Property<ExecutionContext, std::size_t> {};


    /// Required alignment of caller-supplied execution-stack storage.
    struct StackStorageAlignment final : Framework::Property<ExecutionContext, std::size_t> {};


    /// Native stack allocation granularity in bytes.
    struct StackAllocationGranularityBytes final : Framework::Property<ExecutionContext, std::size_t> {};


    /// Native wait resolution used by execution-context join operations.
    struct JoinWaitResolutionNanoseconds final : Framework::Property<ExecutionContext, std::uint64_t> {};

} // ESPressio::Platform::Execution
