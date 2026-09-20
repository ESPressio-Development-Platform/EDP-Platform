#pragma once

#include <type_traits>
#include <utility>

#include "../synchronization/WaitTimeout.hpp"
#include "ExecutionComposition.hpp"
#include "ExecutionTypes.hpp"

namespace ESPressio::Platform::Execution::Detail {

    /// Validates the complete compile-time contract required from an ExecutionContext provider.
    /// @tparam TExecutionContextProvider Concrete provider Type being validated against the ExecutionContext capability contract.
    template<class TExecutionContextProvider>
    struct ExecutionContextProviderTraits {

        static_assert(
            TExecutionContextProvider::CompositionCapabilities::template Contains<ExecutionContext>,
            "ExecutionContext provider must supply the ExecutionContext capability"
        );

        /// Properties advertised for the ExecutionContext capability.
        using Properties = typename TExecutionContextProvider::CompositionCapabilities::template PropertiesFor<ExecutionContext>;

        static_assert(
            Properties::template Contains<CallerSuppliedStorage>,
            "ExecutionContext provider must advertise CallerSuppliedStorage"
        );

        static_assert(
            Properties::template Value<CallerSuppliedStorage>,
            "ExecutionContext provider must use caller-supplied native storage"
        );

        static_assert(
            Properties::template Contains<SupportsPriority> &&
            Properties::template Contains<SupportsProcessorAffinity> &&
            Properties::template Contains<SupportsStackTelemetry>,
            "ExecutionContext provider must advertise optional execution features"
        );

        static_assert(
            Properties::template Contains<ControlStorageBytes> &&
            Properties::template Contains<ControlStorageAlignment> &&
            Properties::template Contains<StackStorageAlignment> &&
            Properties::template Contains<StackAllocationGranularityBytes>,
            "ExecutionContext provider must advertise static storage requirements"
        );

        static_assert(
            Properties::template Value<ControlStorageAlignment> > 0U &&
            Properties::template Value<StackStorageAlignment> > 0U &&
            Properties::template Value<StackAllocationGranularityBytes> > 0U,
            "ExecutionContext provider storage alignments and stack granularity must be positive"
        );

        static_assert(
            Properties::template Contains<JoinWaitResolutionNanoseconds>,
            "ExecutionContext provider must advertise JoinWaitResolutionNanoseconds"
        );

        static_assert(
            Properties::template Value<JoinWaitResolutionNanoseconds> > 0U,
            "ExecutionContext provider join resolution must be greater than zero"
        );

        static_assert(
            std::is_default_constructible_v<TExecutionContextProvider>,
            "ExecutionContext providers must be default constructible"
        );

        static_assert(
            !std::is_copy_constructible_v<TExecutionContextProvider> &&
            !std::is_copy_assignable_v<TExecutionContextProvider>,
            "ExecutionContext providers must not be copyable"
        );

        static_assert(
            !std::is_move_constructible_v<TExecutionContextProvider> &&
            !std::is_move_assignable_v<TExecutionContextProvider>,
            "ExecutionContext providers must not be movable"
        );

        /// Return type produced by execution-context initialization.
        using InitializeResult = decltype(
            std::declval<TExecutionContextProvider&>().Initialize(
                std::declval<const ExecutionStorage&>(),
                std::declval<const ExecutionConfiguration&>(),
                std::declval<ExecutionEntry>(),
                std::declval<void*>()
            )
        );

        /// Return type produced by execution-context start.
        using StartResult = decltype(
            std::declval<TExecutionContextProvider&>().Start()
        );

        /// Return type produced by execution-context join.
        using JoinResult = decltype(
            std::declval<TExecutionContextProvider&>().Join(
                std::declval<Synchronization::WaitTimeout>()
            )
        );

        /// Return type produced by execution-context destruction.
        using DestroyResult = decltype(
            std::declval<TExecutionContextProvider&>().Destroy()
        );

        /// Return type produced by current-context identity inspection.
        using CurrentContextResult = decltype(
            std::declval<const TExecutionContextProvider&>().IsCurrentContext()
        );

        /// Return type produced by stack telemetry inspection.
        using StackTelemetryResult = decltype(
            std::declval<const TExecutionContextProvider&>().GetStackTelemetry()
        );

        /// Return type produced by yielding the selected execution runtime.
        using YieldResult = decltype(
            TExecutionContextProvider::Yield()
        );

        static_assert(
            std::is_same_v<InitializeResult, ExecutionInitializationResult>,
            "ExecutionContext Initialize must return ExecutionInitializationResult"
        );

        static_assert(
            std::is_same_v<StartResult, ExecutionStartResult>,
            "ExecutionContext Start must return ExecutionStartResult"
        );

        static_assert(
            std::is_same_v<JoinResult, ExecutionJoinResult>,
            "ExecutionContext Join must return ExecutionJoinResult"
        );

        static_assert(
            std::is_same_v<DestroyResult, ExecutionDestroyResult>,
            "ExecutionContext Destroy must return ExecutionDestroyResult"
        );

        static_assert(
            std::is_same_v<CurrentContextResult, bool>,
            "ExecutionContext IsCurrentContext must return bool"
        );

        static_assert(
            std::is_same_v<StackTelemetryResult, ExecutionStackTelemetry>,
            "ExecutionContext GetStackTelemetry must return ExecutionStackTelemetry"
        );

        static_assert(
            std::is_same_v<YieldResult, void>,
            "ExecutionContext Yield must return void"
        );

        static_assert(
            noexcept(
                TExecutionContextProvider::Yield()
            ),
            "ExecutionContext Yield must be noexcept"
        );

    };

} // ESPressio::Platform::Execution::Detail
