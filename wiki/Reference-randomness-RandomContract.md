# src/randomness/RandomContract.hpp

**Primary classification:** INTERNAL PROVIDER API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/randomness/RandomContract.hpp)

## Direct includes

- `cstddef`
- `cstdint`
- `type_traits`
- `utility`
- `RandomComposition.hpp`
- `RandomTypes.hpp`

## Documented declarations

### `TRandomByteSourceProvider`

**Classification:** INTERNAL PROVIDER API

Validates the public contract required from a RandomByteSource provider.

- **Template parameter `TRandomByteSourceProvider`:** Concrete provider type being validated.

```cpp
template<class TRandomByteSourceProvider>
    struct RandomByteSourceProviderTraits
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for RandomByteSource.

```cpp
using Properties = typename TRandomByteSourceProvider::CompositionOffers::template PropertiesFor<RandomByteSource>;
```

### `FillResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by random-byte generation.

```cpp
using FillResult = decltype(
            std::declval<TRandomByteSourceProvider&>().FillRandomBytes(
                std::declval<std::uint8_t*>(),
                std::declval<std::size_t>()
            )
        );
```

