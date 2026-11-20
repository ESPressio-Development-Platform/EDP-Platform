#include <ESPressio_Platform_CurrentContextBinding.hpp>

#ifndef EDP_BINDING_NEGATIVE_CASE
#define EDP_BINDING_NEGATIVE_CASE 0
#endif

namespace ESPressio::Platform::Execution::Tests {

    /// Domain-neutral contract shape, with no actual task or interrupt qualification.
    struct Record final {

        /// A domain may overload address-of; binding must still take the object's real address.
        const Record* operator &() const noexcept {
            return nullptr;
        }

    };

    /// Metadata/operation fixture; each negative mode violates exactly one contract boundary.
    struct ShapeProvider final : Framework::Provider<
        Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                CurrentContextBinding,
#if EDP_BINDING_NEGATIVE_CASE != 1
                Framework::PropertyValue<ConstantTimeLookup, EDP_BINDING_NEGATIVE_CASE != 2>,
#endif
                Framework::PropertyValue<AllocationFreeLookup, true>,
                Framework::PropertyValue<CurrentContextOnly, true>,
                Framework::PropertyValue<RejectsInterruptContexts, true>
            >
        >
    > {

        /// Ordinary unqualified object Type except in the explicit invalid-record test.
#if EDP_BINDING_NEGATIVE_CASE == 9
        using ContextRecord = const Record;
#else
        using ContextRecord = Record;
#endif

        /// Exact result vocabulary except in the explicit wrong-record test.
#if EDP_BINDING_NEGATIVE_CASE == 4
        using ReadResult = CurrentContextReadResult<int>;
#else
        using ReadResult = CurrentContextReadResult<ContextRecord>;
#endif

        /// The fixture has a default provider instance.
        ShapeProvider() noexcept = default;

        /// Copying is allowed only in the negative lifetime-shape test.
#if EDP_BINDING_NEGATIVE_CASE == 10
        ShapeProvider(const ShapeProvider&) = default;
#else
        ShapeProvider(const ShapeProvider&) = delete;
#endif

        /// Provider assignment is always forbidden.
        ShapeProvider& operator =(const ShapeProvider&) = delete;

        /// Provider movement is always forbidden.
        ShapeProvider(ShapeProvider&&) = delete;

        /// Provider move assignment is always forbidden.
        ShapeProvider& operator =(ShapeProvider&&) = delete;

        /// Dedicated binding outcome, except for the negative generic-boolean test.
#if EDP_BINDING_NEGATIVE_CASE == 3
        bool
#else
        CurrentContextBindResult
#endif
        BindCurrent(const ContextRecord&) noexcept(EDP_BINDING_NEGATIVE_CASE != 6) {
#if EDP_BINDING_NEGATIVE_CASE == 3
            return false;
#else
            return CurrentContextBindResult::UnsupportedContext;
#endif
        }

        /// Typed lookup with no exception except in its negative exception test.
        ReadResult ReadCurrent() const noexcept(EDP_BINDING_NEGATIVE_CASE != 7) {
            return ReadResult::ForUnsupportedContext();
        }

        /// Expected-record retirement with dedicated status except in the negative type test.
#if EDP_BINDING_NEGATIVE_CASE == 5
        bool
#else
        CurrentContextRetireResult
#endif
        RetireCurrent(const ContextRecord&) noexcept(EDP_BINDING_NEGATIVE_CASE != 8) {
#if EDP_BINDING_NEGATIVE_CASE == 5
            return false;
#else
            return CurrentContextRetireResult::UnsupportedContext;
#endif
        }

    };

    static_assert(sizeof(Detail::CurrentContextBindingProviderTraits<ShapeProvider>) > 0U);

} // ESPressio::Platform::Execution::Tests


/// Exercises only structured result coupling; the shape provider does not model native execution.
int main() {
    using namespace ESPressio::Platform::Execution;
    using ESPressio::Platform::Execution::Tests::Record;
    const Record record;
    const auto bound = CurrentContextReadResult<Record>::ForBound(record);
    const auto unbound = CurrentContextReadResult<Record>::ForUnbound();
    const auto unsupported = CurrentContextReadResult<Record>::ForUnsupportedContext();
    return bound.IsBound() &&
        bound.GetIfBound() == std::addressof(record) &&
        !unbound.IsBound() &&
        unbound.GetIfBound() == nullptr &&
        !unsupported.IsBound() &&
        unsupported.GetIfBound() == nullptr ? 0 : 1;
}
