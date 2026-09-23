# src/synchronization/SynchronizationComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/synchronization/SynchronizationComposition.hpp)

## Direct includes

- `cstdint`
- `../platform/PlatformComposition.hpp`

## Documented declarations

### `Mutex`

**Classification:** PUBLIC COMPOSITION API

Exclusive Platform capability supplying a non-recursive mutex implementation.

```cpp
struct Mutex final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `RecursiveMutex`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Exclusive Platform capability supplying a recursive mutex implementation.

```cpp
struct RecursiveMutex final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `ReadWriteMutex`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Exclusive Platform capability supplying a shared-read/exclusive-write mutex implementation.

```cpp
struct ReadWriteMutex final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `CountingSemaphore`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Exclusive Platform capability supplying a counting semaphore implementation.

```cpp
struct CountingSemaphore final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `Signal`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Exclusive Platform capability supplying a latched wake signal implementation.

```cpp
struct Signal final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `SpinLock`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Exclusive Platform capability supplying a short-duration spin lock implementation.

```cpp
struct SpinLock final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `MutexWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution, in nanoseconds, advertised by a Mutex provider.

```cpp
struct MutexWaitResolutionNanoseconds final : Framework::Property<Mutex, std::uint64_t> {};
```

### `RecursiveMutexWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution, in nanoseconds, advertised by a RecursiveMutex provider.

```cpp
struct RecursiveMutexWaitResolutionNanoseconds final : Framework::Property<RecursiveMutex, std::uint64_t> {};
```

### `ReadWriteMutexWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution, in nanoseconds, advertised by a ReadWriteMutex provider.

```cpp
struct ReadWriteMutexWaitResolutionNanoseconds final : Framework::Property<ReadWriteMutex, std::uint64_t> {};
```

### `CountingSemaphoreWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution, in nanoseconds, advertised by a CountingSemaphore provider.

```cpp
struct CountingSemaphoreWaitResolutionNanoseconds final : Framework::Property<CountingSemaphore, std::uint64_t> {};
```

### `CountingSemaphoreSupportsInterruptRelease`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether a CountingSemaphore provider supports interrupt-context release.

```cpp
struct CountingSemaphoreSupportsInterruptRelease final : Framework::Property<CountingSemaphore, bool> {};
```

### `SignalWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution, in nanoseconds, advertised by a Signal provider.

```cpp
struct SignalWaitResolutionNanoseconds final : Framework::Property<Signal, std::uint64_t> {};
```

### `SignalSupportsInterruptNotification`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether a Signal provider supports interrupt-context notification.

```cpp
struct SignalSupportsInterruptNotification final : Framework::Property<Signal, bool> {};
```

### `SpinLockSupportsInterruptContext`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether a SpinLock provider supports interrupt-context acquisition and release.

```cpp
struct SpinLockSupportsInterruptContext final : Framework::Property<SpinLock, bool> {};
```

