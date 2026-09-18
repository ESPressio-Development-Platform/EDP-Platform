# Platform Synchronization

## Purpose and scope

`EDP-Platform` defines portable synchronization contracts without embedding Arduino, ESP-IDF, FreeRTOS, Standard C++, or operating-system implementations into the abstraction library.

The current capability family is:

```text
Mutex
RecursiveMutex
ReadWriteMutex
CountingSemaphore
Signal
SpinLock
```

All are exclusive capabilities in `ESPressio::Platform::Domain`; application Composition selects one provider for each capability used by the build.

## Provider object contract

Every concrete synchronization provider validated by the Platform traits is:

```text
default constructible
non-copyable
non-movable
```

This keeps native synchronization state identity stable. Composition resolves provider Types but does not construct or own provider objects.

## WaitTimeout

Blocking Platform operations consume the relative `WaitTimeout` value.

It has three normalized modes:

```text
NoWait
Finite nanoseconds
Forever
```

`WaitTimeout::ForNanoseconds(0)` normalizes to `NoWait`.

`Nanoseconds()` returns the finite relative duration, or zero for non-finite modes.

Concrete providers advertise their native wait resolution and are responsible for mapping nanoseconds to the underlying runtime without overstating their precision.

## Wait-resolution properties

Providers advertise capability-specific wait resolution:

```text
MutexWaitResolutionNanoseconds
RecursiveMutexWaitResolutionNanoseconds
ReadWriteMutexWaitResolutionNanoseconds
CountingSemaphoreWaitResolutionNanoseconds
SignalWaitResolutionNanoseconds
```

Consumers with precision requirements must express them through Composition rather than assuming every provider has the same scheduler/timer resolution.

## Mutex

`Mutex` represents a non-recursive exclusive lock.

Required surface:

```text
Acquire(WaitTimeout) -> LockAcquireResult
Release()            -> LockReleaseResult
```

Acquisition distinguishes:

```text
Acquired
TimedOut
ProviderFailure
```

Release distinguishes:

```text
Released
InvalidOwnership
ProviderFailure
```

Correct ownership and release pairing remain part of the synchronization contract even when a particular native implementation cannot diagnose every misuse safely.

## RecursiveMutex

`RecursiveMutex` has the same acquire/release result vocabulary but permits repeated acquisition by the owning execution context according to the selected provider's native recursive-lock semantics.

Each successful recursive acquisition requires a corresponding release.

## ReadWriteMutex

`ReadWriteMutex` provides:

```text
AcquireRead(WaitTimeout)
ReleaseRead()
AcquireWrite(WaitTimeout)
ReleaseWrite()
```

Read acquisition is shared; write acquisition is exclusive.

The Platform abstraction does not impose one fairness algorithm. A concrete provider must document any writer-preference, reader-preference, or implementation-defined fairness behavior that matters to consumers.

## CountingSemaphore

`CountingSemaphore` is explicitly initialized with a maximum permit count and an initial permit count.

Initialization result states are:

```text
Succeeded
InvalidMaximumCount
InvalidInitialCount
AlreadyInitialized
ProviderFailure
```

Permit acquisition reports:

```text
Acquired
TimedOut
NotInitialized
ProviderFailure
```

Permit release reports:

```text
Released
CapacityReached
NotInitialized
UnsupportedInterruptContext
ProviderFailure
```

Providers also advertise:

```text
CountingSemaphoreSupportsInterruptRelease
```

A consumer requiring ISR/interrupt release must constrain that property before calling `ReleaseFromInterrupt()`.

## Signal

`Signal` is a latched, coalescing wake primitive.

`Notify()` establishes readiness. Repeated notifications may coalesce while the signal remains latched.

A successful `Wait()` consumes the current latch.

Signal therefore communicates readiness, not work cardinality. Queue depth, permit count, or item count must remain in the protected domain data structure rather than being inferred from notification count.

Notify result states are:

```text
Signaled
UnsupportedInterruptContext
ProviderFailure
```

Wait result states are:

```text
Signaled
TimedOut
ProviderFailure
```

Providers advertise:

```text
SignalSupportsInterruptNotification
```

Consumers requiring interrupt notification must constrain that property before using `NotifyFromInterrupt()`.

## SpinLock

`SpinLock` is intended only for very short critical sections.

Ordinary acquisition is:

```cpp
void Acquire() noexcept;
```

rather than a timed wait. It blocks/spins according to the concrete provider until ownership is established.

Release returns `SpinLockReleaseResult`.

Interrupt-context operations return explicit result enums and are governed by:

```text
SpinLockSupportsInterruptContext
```

A spin lock must never be treated as a general mutex replacement and must not protect blocking, yielding, long-running, or progress-dependent work.

## Interrupt-context boundary

Interrupt support is never inferred from the capability name or target platform.

It is explicit only where a capability exposes a corresponding Composition property and interrupt-specific method.

Current explicit interrupt surfaces are:

```text
CountingSemaphore::ReleaseFromInterrupt
Signal::NotifyFromInterrupt
SpinLock::AcquireFromInterrupt / ReleaseFromInterrupt
```

Mutex, RecursiveMutex, and ReadWriteMutex do not expose interrupt-context operations.

## Memory and resource ownership

`EDP-Platform` owns only the contracts and value Types. It does not allocate native synchronization objects.

Concrete provider repositories own the implementation resource model. One provider may use fixed inline/static RTOS storage while another may rely on Standard Library/OS objects whose internal resource use is implementation-defined.

Consumers that require deterministic static storage must select a provider whose documented concrete contract supplies that guarantee.

## Concurrency semantics

The synchronization objects are themselves the concurrency boundary; they contain whatever native state the selected provider requires.

No Platform contract introduces a hidden worker task, callback registry, global service locator, or implicit scheduler.

## Concrete ownership

Provider ownership follows the API that actually implements the abstraction:

```text
FreeRTOS API    -> EDP-Platform-FreeRTOS
ESP-IDF API     -> EDP-Platform-ESP-IDF
Arduino API     -> EDP-Platform-Arduino
Standard C++    -> EDP-Platform-Portable
```

A framework that embeds another runtime does not transfer provider ownership. For example, a FreeRTOS mutex used by an Arduino-ESP32 application remains a FreeRTOS provider.

## Explicit non-goals

This synchronization layer does not define:

```text
Task or Thread lifecycle
scheduler policy
work queues
future/promise semantics
condition-variable capability
event-group capability
automatic ownership tracking beyond provider/native guarantees
deadlock detection
priority-inheritance policy beyond the selected native primitive
```

Additional capabilities should be introduced only when a concrete ESPressio consumer requires a portable contract for them.
