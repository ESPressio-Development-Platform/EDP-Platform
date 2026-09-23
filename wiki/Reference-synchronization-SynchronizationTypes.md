# src/synchronization/SynchronizationTypes.hpp

**Primary classification:** PUBLIC API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/synchronization/SynchronizationTypes.hpp)

## Direct includes

- `cstdint`

## Documented declarations

### `LockAcquireResult`

**Classification:** PUBLIC API

Outcome from acquiring an exclusive or shared lock.

```cpp
enum class LockAcquireResult : std::uint8_t
```

### `LockReleaseResult`

**Classification:** PUBLIC API

Outcome from releasing an exclusive or shared lock.

```cpp
enum class LockReleaseResult : std::uint8_t
```

### `SemaphoreInitializationResult`

**Classification:** PUBLIC API

Outcome from initializing a counting semaphore.

```cpp
enum class SemaphoreInitializationResult : std::uint8_t
```

### `SemaphoreAcquireResult`

**Classification:** PUBLIC API

Outcome from acquiring one counting-semaphore permit.

```cpp
enum class SemaphoreAcquireResult : std::uint8_t
```

### `SemaphoreReleaseResult`

**Classification:** PUBLIC API

Outcome from releasing one counting-semaphore permit.

```cpp
enum class SemaphoreReleaseResult : std::uint8_t
```

### `SignalNotifyResult`

**Classification:** PUBLIC API

Outcome from notifying a latched signal.

```cpp
enum class SignalNotifyResult : std::uint8_t
```

### `SignalWaitResult`

**Classification:** PUBLIC API

Outcome from waiting for a latched signal.

```cpp
enum class SignalWaitResult : std::uint8_t
```

### `SpinLockAcquireResult`

**Classification:** PUBLIC API

Outcome from acquiring a spin lock in interrupt context.

```cpp
enum class SpinLockAcquireResult : std::uint8_t
```

### `SpinLockReleaseResult`

**Classification:** PUBLIC API

Outcome from releasing a spin lock.

```cpp
enum class SpinLockReleaseResult : std::uint8_t
```

