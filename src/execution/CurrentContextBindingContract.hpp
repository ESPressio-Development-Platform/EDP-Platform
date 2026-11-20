#pragma once

#include <type_traits>
#include <utility>

#include "CurrentContextBindingComposition.hpp"
#include "CurrentContextBindingTypes.hpp"

namespace ESPressio::Platform::Execution::Detail {

    /// Validates an explicitly selected current-context binding provider without changing execution defaults.
    /// @tparam TBindingProvider Concrete provider with one owner-selected borrowed record Type.
    template<class TBindingProvider>
    struct CurrentContextBindingProviderTraits {

        static_assert(
            TBindingProvider::CompositionOffers::template Contains<CurrentContextBinding>,
            "Binding provider must supply CurrentContextBinding"
        );

        // Selected provider metadata.

        /// Capability properties for this concrete binding provider.
        using Properties = typename TBindingProvider::CompositionOffers::template PropertiesFor<CurrentContextBinding>;

        /// Domain-neutral record whose lifetime remains with the caller.
        using ContextRecord = typename TBindingProvider::ContextRecord;

        /// Exact structured lookup result required for the selected record.
        using ReadResult = CurrentContextReadResult<ContextRecord>;

        static_assert(
            std::is_object_v<ContextRecord> &&
            std::is_same_v<ContextRecord, std::remove_cv_t<ContextRecord>>,
            "Binding ContextRecord must be an unqualified object Type"
        );

        static_assert(
            Properties::template Contains<ConstantTimeLookup> &&
            Properties::template Contains<AllocationFreeLookup> &&
            Properties::template Contains<CurrentContextOnly> &&
            Properties::template Contains<RejectsInterruptContexts>,
            "Binding provider must advertise every bounded current-context property"
        );

        static_assert(
            Properties::template Value<ConstantTimeLookup> &&
            Properties::template Value<AllocationFreeLookup> &&
            Properties::template Value<CurrentContextOnly> &&
            Properties::template Value<RejectsInterruptContexts>,
            "Binding provider must satisfy every bounded current-context property"
        );

        static_assert(
            std::is_default_constructible_v<TBindingProvider> &&
            !std::is_copy_constructible_v<TBindingProvider> &&
            !std::is_copy_assignable_v<TBindingProvider> &&
            !std::is_move_constructible_v<TBindingProvider> &&
            !std::is_move_assignable_v<TBindingProvider>,
            "Binding provider must be default constructible and noncopyable/nonmovable"
        );

        // Exact operation result types and exception boundaries.

        /// Dedicated binding outcome, never a generic boolean.
        using BindResult = decltype(
            std::declval<TBindingProvider&>().BindCurrent(
                std::declval<const ContextRecord&>()
            )
        );

        /// Typed status coupled to the current borrowed record.
        using ActualReadResult = decltype(
            std::declval<const TBindingProvider&>().ReadCurrent()
        );

        /// Dedicated expected-record retirement outcome.
        using RetireResult = decltype(
            std::declval<TBindingProvider&>().RetireCurrent(
                std::declval<const ContextRecord&>()
            )
        );

        static_assert(
            std::is_same_v<BindResult, CurrentContextBindResult>,
            "BindCurrent must return CurrentContextBindResult"
        );

        static_assert(
            std::is_same_v<ActualReadResult, ReadResult>,
            "ReadCurrent must return the exact CurrentContextReadResult"
        );

        static_assert(
            std::is_same_v<RetireResult, CurrentContextRetireResult>,
            "RetireCurrent must return CurrentContextRetireResult"
        );

        static_assert(
            noexcept(
                std::declval<TBindingProvider&>().BindCurrent(
                    std::declval<const ContextRecord&>()
                )
            ),
            "BindCurrent must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<const TBindingProvider&>().ReadCurrent()
            ),
            "ReadCurrent must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<TBindingProvider&>().RetireCurrent(
                    std::declval<const ContextRecord&>()
                )
            ),
            "RetireCurrent must be noexcept"
        );

    };

} // ESPressio::Platform::Execution::Detail
