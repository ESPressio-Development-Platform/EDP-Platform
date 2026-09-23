# src/randomness/RandomComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/randomness/RandomComposition.hpp)

## Direct includes

- `cstdint`
- `../platform/PlatformComposition.hpp`

## Documented declarations

### `RandomByteSource`

**Classification:** PUBLIC COMPOSITION API

Shared Platform capability supplying unpredictable bytes.

```cpp
struct RandomByteSource final : Framework::SharedCapability<Platform::Domain> {};
```

### `RandomByteSourceFeature`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Finite features advertised by one RandomByteSource provider.

```cpp
enum class RandomByteSourceFeature : std::uint8_t
```

### `RandomByteSourceFeatures`

**Classification:** PUBLIC COMPOSITION API

Feature set advertised by one RandomByteSource provider.

```cpp
struct RandomByteSourceFeatures final : Framework::FlagProperty<
        RandomByteSource,
```

### `CryptographicRandomByteSourceConstraint`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Qualification requiring a RandomByteSource suitable for cryptographic use.

```cpp
using CryptographicRandomByteSourceConstraint = Framework::HasAllFlags<
        RandomByteSourceFeatures,
```

### `CryptographicRandomByteSourceRequirement`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Standalone consumer Requirement selecting cryptographically suitable randomness.

```cpp
using CryptographicRandomByteSourceRequirement = Framework::Requirement<
        RandomByteSource,
```

