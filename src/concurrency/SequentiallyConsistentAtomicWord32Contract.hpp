#pragma once

#include "AtomicWord32Contract.hpp"

namespace ESPressio::Platform::Concurrency::Detail {

    /// Validates the optional cross-word sequentially consistent load/store contract.
    /// @tparam TAtomicWordProvider Provider advertising the stronger ordering property.
    template<class TAtomicWordProvider>
    struct SequentiallyConsistentAtomicWord32ProviderTraits :
        AtomicWord32ProviderTraits<TAtomicWordProvider> {

        // Base provider metadata.

        /// Existing base contract, preserved for providers which do not opt in.
        using Base = AtomicWord32ProviderTraits<TAtomicWordProvider>;

        /// Composition properties advertised by the selected atomic-word provider.
        using Properties = typename Base::Properties;

        /// The same four-byte word used by the existing base contract.
        using Word = typename Base::Word;

        static_assert(
            Properties::template Contains<SupportsSequentiallyConsistentLoadStore>,
            "AtomicWord32 provider must advertise SupportsSequentiallyConsistentLoadStore"
        );

        static_assert(
            Properties::template Value<SupportsSequentiallyConsistentLoadStore>,
            "AtomicWord32 provider must support sequentially consistent loads and stores"
        );

        // Strong operation result types.

        /// Exact unsigned word value returned by the stronger load.
        using SequentialLoadResult = decltype(
            std::declval<const Word&>().LoadSequentiallyConsistent()
        );

        /// No operational status is required for an infallible qualified atomic store.
        using SequentialStoreResult = decltype(
            std::declval<Word&>().StoreSequentiallyConsistent(
                std::declval<std::uint32_t>()
            )
        );

        static_assert(
            std::is_same_v<SequentialLoadResult, std::uint32_t>,
            "Sequentially consistent load must return std::uint32_t"
        );

        static_assert(
            std::is_same_v<SequentialStoreResult, void>,
            "Sequentially consistent store must return void"
        );

        static_assert(
            noexcept(
                std::declval<const Word&>().LoadSequentiallyConsistent()
            ),
            "Sequentially consistent load must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<Word&>().StoreSequentiallyConsistent(
                    std::declval<std::uint32_t>()
                )
            ),
            "Sequentially consistent store must be noexcept"
        );

    };

} // ESPressio::Platform::Concurrency::Detail
