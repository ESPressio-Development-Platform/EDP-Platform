#pragma once

#include <cstdint>
#include <cstdio>
#include <ESPressio_Platform_CurrentContextBinding.hpp>

namespace CurrentContextBindingResultsDemo {

    /// App-owned example record; no concrete execution provider is selected here.
    struct Record final {

        /// Domain-defined logical identity.
        std::uint64_t identity;

    };

    /// Shows that unavailable results expose no stale payload.
    inline void Run() {
        using ESPressio::Platform::Execution::CurrentContextReadResult;
        const Record record{1U};
        const auto bound = CurrentContextReadResult<Record>::ForBound(record);
        const auto unbound = CurrentContextReadResult<Record>::ForUnbound();

        std::printf(
            "Bound=%u; unavailable payload absent=%u\n",
            bound.GetIfBound() == &record ? 1U : 0U,
            unbound.GetIfBound() == nullptr ? 1U : 0U
        );
    }

} // CurrentContextBindingResultsDemo
