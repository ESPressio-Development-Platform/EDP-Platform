# src/concurrency/ConcurrencyComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API

**Source:** [current source](../src/concurrency/ConcurrencyComposition.hpp)


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



## Optional strong-ordering extension

PUBLIC COMPOSITION property SupportsSequentiallyConsistentLoadStore belongs to AtomicWord32. True promises a single sequentially consistent order across participating strong operations on the provider's words. Absent/false leaves the original contract. The concurrency umbrella also exposes the optional [provider validator](Reference-concurrency-SequentiallyConsistentAtomicWord32Contract); it adds no mandatory methods to base providers. See [normative semantics](../docs/SEQUENTIALLY-CONSISTENT-ATOMIC-WORD.MD).
