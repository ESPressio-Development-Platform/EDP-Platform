# src/synchronization/SynchronizationContracts.hpp

**Primary classification:** INTERNAL PROVIDER API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/synchronization/SynchronizationContracts.hpp)

## Direct includes

- `type_traits`
- `utility`
- `SynchronizationComposition.hpp`
- `SynchronizationTypes.hpp`
- `WaitTimeout.hpp`

## Documented declarations

### `TSynchronizationProvider`

**Classification:** INTERNAL PROVIDER API

Validates the common object properties required from a synchronization provider.

```cpp
template<class TSynchronizationProvider>
    struct SynchronizationObjectTraits
```

### `TMutexProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a Mutex capability provider.

```cpp
template<class TMutexProvider>
    struct MutexProviderTraits : SynchronizationObjectTraits<TMutexProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the Mutex capability.

```cpp
using Properties = typename TMutexProvider::CompositionOffers::template PropertiesFor<Mutex>;
```

### `AcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by mutex acquisition.

```cpp
using AcquireResult = decltype(
            std::declval<TMutexProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );
```

### `ReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by mutex release.

```cpp
using ReleaseResult = decltype(
            std::declval<TMutexProvider&>().Release()
        );
```

### `TRecursiveMutexProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a RecursiveMutex capability provider.

```cpp
template<class TRecursiveMutexProvider>
    struct RecursiveMutexProviderTraits : SynchronizationObjectTraits<TRecursiveMutexProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the RecursiveMutex capability.

```cpp
using Properties = typename TRecursiveMutexProvider::CompositionOffers::template PropertiesFor<RecursiveMutex>;
```

### `AcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by recursive-mutex acquisition.

```cpp
using AcquireResult = decltype(
            std::declval<TRecursiveMutexProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );
```

### `ReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by recursive-mutex release.

```cpp
using ReleaseResult = decltype(
            std::declval<TRecursiveMutexProvider&>().Release()
        );
```

### `TReadWriteMutexProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a ReadWriteMutex capability provider.

```cpp
template<class TReadWriteMutexProvider>
    struct ReadWriteMutexProviderTraits : SynchronizationObjectTraits<TReadWriteMutexProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the ReadWriteMutex capability.

```cpp
using Properties = typename TReadWriteMutexProvider::CompositionOffers::template PropertiesFor<ReadWriteMutex>;
```

### `ReadAcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by shared-read acquisition.

```cpp
using ReadAcquireResult = decltype(
            std::declval<TReadWriteMutexProvider&>().AcquireRead(
                std::declval<WaitTimeout>()
            )
        );
```

### `ReadReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by shared-read release.

```cpp
using ReadReleaseResult = decltype(
            std::declval<TReadWriteMutexProvider&>().ReleaseRead()
        );
```

### `WriteAcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by exclusive-write acquisition.

```cpp
using WriteAcquireResult = decltype(
            std::declval<TReadWriteMutexProvider&>().AcquireWrite(
                std::declval<WaitTimeout>()
            )
        );
```

### `WriteReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by exclusive-write release.

```cpp
using WriteReleaseResult = decltype(
            std::declval<TReadWriteMutexProvider&>().ReleaseWrite()
        );
```

### `TCountingSemaphoreProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a CountingSemaphore capability provider.

```cpp
template<class TCountingSemaphoreProvider>
    struct CountingSemaphoreProviderTraits : SynchronizationObjectTraits<TCountingSemaphoreProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the CountingSemaphore capability.

```cpp
using Properties = typename TCountingSemaphoreProvider::CompositionOffers::template PropertiesFor<CountingSemaphore>;
```

### `InitializeResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by semaphore initialization.

```cpp
using InitializeResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Initialize(
                std::declval<std::uint32_t>(),
                std::declval<std::uint32_t>()
            )
        );
```

### `AcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by permit acquisition.

```cpp
using AcquireResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );
```

### `ReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by permit release.

```cpp
using ReleaseResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Release()
        );
```

### `InterruptReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by interrupt-context permit release.

```cpp
using InterruptReleaseResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().ReleaseFromInterrupt()
        );
```

### `TSignalProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a Signal capability provider.

```cpp
template<class TSignalProvider>
    struct SignalProviderTraits : SynchronizationObjectTraits<TSignalProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the Signal capability.

```cpp
using Properties = typename TSignalProvider::CompositionOffers::template PropertiesFor<Signal>;
```

### `NotifyResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by ordinary signal notification.

```cpp
using NotifyResult = decltype(
            std::declval<TSignalProvider&>().Notify()
        );
```

### `InterruptNotifyResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by interrupt-context signal notification.

```cpp
using InterruptNotifyResult = decltype(
            std::declval<TSignalProvider&>().NotifyFromInterrupt()
        );
```

### `WaitResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by waiting for a signal.

```cpp
using WaitResult = decltype(
            std::declval<TSignalProvider&>().Wait(
                std::declval<WaitTimeout>()
            )
        );
```

### `TSpinLockProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete contract required from a SpinLock capability provider.

```cpp
template<class TSpinLockProvider>
    struct SpinLockProviderTraits : SynchronizationObjectTraits<TSpinLockProvider>
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the SpinLock capability.

```cpp
using Properties = typename TSpinLockProvider::CompositionOffers::template PropertiesFor<SpinLock>;
```

### `AcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by ordinary-context blocking acquisition.

```cpp
using AcquireResult = decltype(
            std::declval<TSpinLockProvider&>().Acquire()
        );
```

### `InterruptAcquireResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by interrupt-context acquisition.

```cpp
using InterruptAcquireResult = decltype(
            std::declval<TSpinLockProvider&>().AcquireFromInterrupt()
        );
```

### `ReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by ordinary-context release.

```cpp
using ReleaseResult = decltype(
            std::declval<TSpinLockProvider&>().Release()
        );
```

### `InterruptReleaseResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by interrupt-context release.

```cpp
using InterruptReleaseResult = decltype(
            std::declval<TSpinLockProvider&>().ReleaseFromInterrupt()
        );
```

