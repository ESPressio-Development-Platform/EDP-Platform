# Platform Synchronization

## Purpose

`EDP-Platform` defines portable synchronization semantics without embedding Arduino, ESP-IDF, FreeRTOS or Standard C++ runtime implementations into the abstraction library.

The initial synchronization capabilities are:

```text
Mutex
RecursiveMutex
ReadWriteMutex
CountingSemaphore
Signal
SpinLock
```

Each capability is exclusive and selected through the ESPressio Composition Framework.

## Dependency boundary

The abstractions depend only on `EDP-System` for Composition.

Concrete providers belong in concrete Platform repositories.

## Wait representation

Blocking operations use `WaitTimeout`.

It supports:

```text
NoWait
finite relative nanoseconds
WaitForever
```

Nanoseconds are the portable representation. A provider with a coarser native clock must round finite waits upward so that a requested timeout does not expire earlier merely because of unit conversion.

## Signal semantics

`Signal` is a latched, coalescing wake primitive.

A notification records readiness. Repeated notifications may coalesce while the signal remains latched.

A successful wait consumes the current latch.

Work cardinality must therefore live in the protected queue, counter, pool or domain state rather than being inferred from notification count.

## Interrupt context

Signal notification and counting-semaphore release expose explicit interrupt-context operations.

Providers advertise interrupt support as Composition properties.

Code that requires interrupt support must constrain the selected provider accordingly.

## Spin locks

Spin locks are intended only for very short critical regions.

They are not a general mutex replacement and must never be used around blocking, yielding or long-running operations.

Interrupt-context support is provider-specific and advertised explicitly.

## Concrete ownership

Concrete implementations belong to the repository matching the API that actually backs the implementation.

Examples:

```text
FreeRTOS API    -> EDP-Platform-FreeRTOS
ESP-IDF API     -> EDP-Platform-ESP-IDF
Arduino API     -> EDP-Platform-Arduino
Standard C++    -> EDP-Platform-Portable
```
