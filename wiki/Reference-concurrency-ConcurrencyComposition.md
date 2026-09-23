# src/concurrency/ConcurrencyComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/concurrency/ConcurrencyComposition.hpp)

## Direct includes

- `cstddef`
- `../platform/PlatformComposition.hpp`

## Documented declarations

### `AtomicWord32`

**Classification:** PUBLIC COMPOSITION API

Exclusive Platform capability supplying an unsigned 32-bit atomic word implementation.

```cpp
struct AtomicWord32 final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `LockFree`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether an AtomicWord32 provider guarantees operation without a hidden lock.

```cpp
struct LockFree final : Framework::Property<AtomicWord32, bool> {};
```

### `AtomicWordStorageBytes`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Number of bytes occupied by one concrete AtomicWord32 object.

```cpp
struct AtomicWordStorageBytes final : Framework::Property<AtomicWord32, std::size_t> {};
```

