# src/concurrency/AtomicWord32Contract.hpp

**Primary classification:** INTERNAL PROVIDER API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/concurrency/AtomicWord32Contract.hpp)

## Direct includes

- `cstdint`
- `type_traits`
- `utility`
- `ConcurrencyComposition.hpp`

## Documented declarations

### `TAtomicWordProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete compile-time contract required from an AtomicWord32 capability provider.
- **Template parameter `TAtomicWordProvider`:** Concrete provider Type being validated against the AtomicWord32 capability contract.

```cpp
template<class TAtomicWordProvider>
    struct AtomicWord32ProviderTraits
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the AtomicWord32 capability.

```cpp
using Properties = typename TAtomicWordProvider::CompositionOffers::template PropertiesFor<AtomicWord32>;
```

### `Word`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Concrete atomic word type supplied by the provider.

```cpp
using Word = typename TAtomicWordProvider::Word;
```

### `RelaxedLoadResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by relaxed atomic loads.

```cpp
using RelaxedLoadResult = decltype(
            std::declval<const Word&>().LoadRelaxed()
        );
```

### `AcquireLoadResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by acquire atomic loads.

```cpp
using AcquireLoadResult = decltype(
            std::declval<const Word&>().LoadAcquire()
        );
```

### `RelaxedStoreResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by relaxed atomic stores.

```cpp
using RelaxedStoreResult = decltype(
            std::declval<Word&>().StoreRelaxed(
                std::declval<std::uint32_t>()
            )
        );
```

### `ReleaseStoreResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by release atomic stores.

```cpp
using ReleaseStoreResult = decltype(
            std::declval<Word&>().StoreRelease(
                std::declval<std::uint32_t>()
            )
        );
```

### `CompareExchangeResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by acquire/release compare-exchange.

```cpp
using CompareExchangeResult = decltype(
            std::declval<Word&>().CompareExchangeAcqRel(
                std::declval<std::uint32_t&>(),
                std::declval<std::uint32_t>()
            )
        );
```

