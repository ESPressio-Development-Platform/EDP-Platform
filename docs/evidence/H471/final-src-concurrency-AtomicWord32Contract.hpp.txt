#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "ConcurrencyComposition.hpp"

namespace ESPressio::Platform::Concurrency::Detail {

    /// Validates the complete compile-time contract required from an AtomicWord32 capability provider.
    /// @tparam TAtomicWordProvider Concrete provider Type being validated against the AtomicWord32 capability contract.
    template<class TAtomicWordProvider>
    struct AtomicWord32ProviderTraits {

        static_assert(
            TAtomicWordProvider::CompositionOffers::template Contains<AtomicWord32>,
            "AtomicWord32 provider must supply the AtomicWord32 capability"
        );

        // Provider metadata.

        /// Properties advertised for the AtomicWord32 capability.
        using Properties = typename TAtomicWordProvider::CompositionOffers::template PropertiesFor<AtomicWord32>;

        static_assert(
            Properties::template Contains<LockFree>,
            "AtomicWord32 provider must advertise the LockFree property"
        );

        static_assert(
            Properties::template Value<LockFree>,
            "AtomicWord32 provider must guarantee lock-free operation"
        );

        static_assert(
            Properties::template Contains<AtomicWordStorageBytes>,
            "AtomicWord32 provider must advertise AtomicWordStorageBytes"
        );

        static_assert(
            Properties::template Value<AtomicWordStorageBytes> == sizeof(std::uint32_t),
            "AtomicWord32 provider must advertise exactly four storage bytes"
        );

        /// Concrete atomic word type supplied by the provider.
        using Word = typename TAtomicWordProvider::Word;

        static_assert(
            sizeof(Word) == sizeof(std::uint32_t),
            "AtomicWord32 provider Word must occupy exactly four bytes"
        );

        static_assert(
            std::is_default_constructible_v<Word>,
            "AtomicWord32 provider Word must be default constructible"
        );

        static_assert(
            !std::is_copy_constructible_v<Word> &&
            !std::is_copy_assignable_v<Word>,
            "AtomicWord32 provider Word must not be copyable"
        );

        static_assert(
            !std::is_move_constructible_v<Word> &&
            !std::is_move_assignable_v<Word>,
            "AtomicWord32 provider Word must not be movable"
        );

        // Operation result types.

        /// Return type produced by relaxed atomic loads.
        using RelaxedLoadResult = decltype(
            std::declval<const Word&>().LoadRelaxed()
        );

        /// Return type produced by acquire atomic loads.
        using AcquireLoadResult = decltype(
            std::declval<const Word&>().LoadAcquire()
        );

        /// Return type produced by relaxed atomic stores.
        using RelaxedStoreResult = decltype(
            std::declval<Word&>().StoreRelaxed(
                std::declval<std::uint32_t>()
            )
        );

        /// Return type produced by release atomic stores.
        using ReleaseStoreResult = decltype(
            std::declval<Word&>().StoreRelease(
                std::declval<std::uint32_t>()
            )
        );

        /// Return type produced by acquire/release compare-exchange.
        using CompareExchangeResult = decltype(
            std::declval<Word&>().CompareExchangeAcqRel(
                std::declval<std::uint32_t&>(),
                std::declval<std::uint32_t>()
            )
        );

        static_assert(
            std::is_same_v<RelaxedLoadResult, std::uint32_t>,
            "AtomicWord32 relaxed loads must return std::uint32_t"
        );

        static_assert(
            std::is_same_v<AcquireLoadResult, std::uint32_t>,
            "AtomicWord32 acquire loads must return std::uint32_t"
        );

        static_assert(
            std::is_same_v<RelaxedStoreResult, void>,
            "AtomicWord32 relaxed stores must return void"
        );

        static_assert(
            std::is_same_v<ReleaseStoreResult, void>,
            "AtomicWord32 release stores must return void"
        );

        static_assert(
            std::is_same_v<CompareExchangeResult, bool>,
            "AtomicWord32 compare-exchange must return bool"
        );

        // Operation exception guarantees.

        static_assert(
            noexcept(
                std::declval<const Word&>().LoadRelaxed()
            ),
            "AtomicWord32 relaxed loads must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<const Word&>().LoadAcquire()
            ),
            "AtomicWord32 acquire loads must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<Word&>().StoreRelaxed(
                    std::declval<std::uint32_t>()
                )
            ),
            "AtomicWord32 relaxed stores must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<Word&>().StoreRelease(
                    std::declval<std::uint32_t>()
                )
            ),
            "AtomicWord32 release stores must be noexcept"
        );

        static_assert(
            noexcept(
                std::declval<Word&>().CompareExchangeAcqRel(
                    std::declval<std::uint32_t&>(),
                    std::declval<std::uint32_t>()
                )
            ),
            "AtomicWord32 compare-exchange must be noexcept"
        );

    };

} // ESPressio::Platform::Concurrency::Detail
