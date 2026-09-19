#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "ConcurrencyComposition.hpp"

namespace ESPressio::Platform::Concurrency::Detail {

    /// Validates the complete compile-time contract required from an AtomicWord8 capability provider.
    template<class TAtomicWordProvider>
    struct AtomicWord8ProviderTraits {

        static_assert(
            TAtomicWordProvider::CompositionCapabilities::template Contains<AtomicWord8>,
            "AtomicWord8 provider must supply the AtomicWord8 capability"
        );

        // Provider metadata.

        /// Properties advertised for the AtomicWord8 capability.
        using Properties = typename TAtomicWordProvider::CompositionCapabilities::template PropertiesFor<AtomicWord8>;

        static_assert(
            Properties::template Contains<AtomicWord8LockFree>,
            "AtomicWord8 provider must advertise AtomicWord8LockFree"
        );

        static_assert(
            Properties::template Value<AtomicWord8LockFree>,
            "AtomicWord8 provider must guarantee lock-free operation"
        );

        static_assert(
            Properties::template Contains<AtomicWord8StorageBytes>,
            "AtomicWord8 provider must advertise AtomicWord8StorageBytes"
        );

        static_assert(
            Properties::template Value<AtomicWord8StorageBytes> == sizeof(std::uint8_t),
            "AtomicWord8 provider must advertise exactly one storage byte"
        );

        /// Concrete atomic byte type supplied by the provider.
        using Word = typename TAtomicWordProvider::Word;

        static_assert(
            sizeof(Word) == sizeof(std::uint8_t),
            "AtomicWord8 provider Word must occupy exactly one byte"
        );

        static_assert(
            std::is_default_constructible_v<Word>,
            "AtomicWord8 provider Word must be default constructible"
        );

        static_assert(
            !std::is_copy_constructible_v<Word> &&
            !std::is_copy_assignable_v<Word> &&
            !std::is_move_constructible_v<Word> &&
            !std::is_move_assignable_v<Word>,
            "AtomicWord8 provider Word must have stable non-copyable, non-movable identity"
        );

        using RelaxedLoadResult = decltype(
            std::declval<const Word&>().LoadRelaxed()
        );

        using AcquireLoadResult = decltype(
            std::declval<const Word&>().LoadAcquire()
        );

        using RelaxedStoreResult = decltype(
            std::declval<Word&>().StoreRelaxed(
                std::declval<std::uint8_t>()
            )
        );

        using ReleaseStoreResult = decltype(
            std::declval<Word&>().StoreRelease(
                std::declval<std::uint8_t>()
            )
        );

        using CompareExchangeResult = decltype(
            std::declval<Word&>().CompareExchangeAcqRel(
                std::declval<std::uint8_t&>(),
                std::declval<std::uint8_t>()
            )
        );

        static_assert(
            std::is_same_v<RelaxedLoadResult, std::uint8_t> &&
            std::is_same_v<AcquireLoadResult, std::uint8_t>,
            "AtomicWord8 loads must return std::uint8_t"
        );

        static_assert(
            std::is_same_v<RelaxedStoreResult, void> &&
            std::is_same_v<ReleaseStoreResult, void>,
            "AtomicWord8 stores must return void"
        );

        static_assert(
            std::is_same_v<CompareExchangeResult, bool>,
            "AtomicWord8 compare-exchange must return bool"
        );

    };

} // ESPressio::Platform::Concurrency::Detail
