# Platform Execution Context

## Purpose and scope

`ExecutionContext` is the lowest ESPressio abstraction for one native schedulable execution context.

It deliberately does **not** define ESPressio Task, Thread, loop, future, pool, cancellation, precision-execution, or scheduler semantics. Those higher semantics belong to higher domains such as `EDP-Threading`.

The Platform abstraction exists to describe the native execution facility that those higher domains may consume without depending directly on FreeRTOS, ESP-IDF, an operating system, or another concrete runtime.

## Composition capability

`ExecutionContext` is an exclusive capability in `ESPressio::Platform::Domain`.

A conforming provider advertises the following properties:

```text
CallerSuppliedStorage
SupportsPriority
SupportsProcessorAffinity
SupportsStackTelemetry
ControlStorageBytes
ControlStorageAlignment
StackStorageAlignment
StackAllocationGranularityBytes
JoinWaitResolutionNanoseconds
```

`CallerSuppliedStorage` is required to be `true`.

`ControlStorageAlignment`, `StackStorageAlignment`, `StackAllocationGranularityBytes`, and `JoinWaitResolutionNanoseconds` must all be greater than zero. `ControlStorageBytes` may legitimately be zero for a provider whose native execution-control state requires no caller-reserved byte region.

A higher domain that requires priority, processor affinity, or stack telemetry must constrain the corresponding Composition property rather than assuming every provider supports that feature.

## Provider object contract

A concrete `ExecutionContext` provider must be:

```text
default constructible
non-copyable
non-movable
```

The provider object therefore has a stable identity and lifetime chosen by ordinary C++ ownership, typically application Bootstrap or a higher execution owner.

Composition resolves the provider Type; it does not construct, retain, start, stop, or destroy the runtime provider object.

## Caller-supplied storage

`ExecutionStorage` carries two caller-owned regions:

```text
ControlAddress + ControlBytes
StackAddress   + StackBytes
```

The selected provider's Composition properties define the minimum control-storage byte requirement, alignment requirements, and native stack allocation granularity.

The caller is responsible for preserving those storage regions for the complete native context lifetime. A provider must reject storage that does not satisfy its advertised contract rather than silently allocating substitute native task/control or stack storage behind the Platform API.

The Platform abstraction does not prescribe where those bytes physically reside. A higher memory/bootstrap layer may place them according to the application's deterministic resource plan.

## Execution configuration

`ExecutionConfiguration` uses Platform-neutral vocabulary:

```text
Priority (Low / Normal / High / Critical)
ProcessorAffinity
Name
```

`ExecutionPriority` is deliberately logical rather than a native scheduler number. Concrete providers map the four levels onto their native priority range. A native runtime with fewer useful levels may collapse adjacent logical levels; consumers must not infer a particular native numeric value.

`ProcessorAffinity` is either:

```text
Any
Specific(zero-based processor index)
```

The optional diagnostic `Name` is a borrowed C string for initialization. Providers may copy or ignore it according to their native runtime contract; consumers must not infer long-term borrowing unless a concrete provider explicitly documents that behavior.

## Entry function

The native entry boundary is:

```cpp
using ExecutionEntry = void (*)(void*) noexcept;
```

The Platform contract therefore does not transport exceptions across the execution-entry boundary.

## Lifecycle

The portable lifecycle surface is:

```text
Initialize
Start
Join
Destroy
```

`Initialize()` binds caller storage, configuration, entry function, and entry parameter.

`Start()` makes an initialized context eligible to execute according to the concrete runtime.

The entry function returns when the higher-level work represented by that native context has completed.

`Join(timeout)` is the ownership synchronization point for waiting until that returned execution is complete.

`Destroy()` retires the native execution context and releases its claim on caller-supplied storage.

The abstraction does not define forced termination. Concrete providers must not invent Threading-level cancellation semantics behind this Platform lifecycle.

## Lifecycle result semantics

Initialization reports:

```text
Succeeded
AlreadyInitialized
InvalidStorage
InvalidConfiguration
UnsupportedAffinity
ProviderFailure
```

Start reports:

```text
Succeeded
InvalidState
ProviderFailure
```

Join reports:

```text
Succeeded
TimedOut
SelfJoin
InvalidState
ProviderFailure
```

Destroy reports:

```text
Succeeded
InvalidState
ProviderFailure
```

These strongly typed results preserve operational distinctions that would be lost behind Boolean success/failure.

## Join waits

`Join()` consumes `ESPressio::Platform::Synchronization::WaitTimeout`.

The provider advertises `JoinWaitResolutionNanoseconds`, allowing higher layers to reject a provider whose native wait precision cannot satisfy their requirement.

Concrete providers are responsible for mapping Platform nanoseconds onto their native wait primitive without claiming precision they do not provide.

## Stack telemetry

`ExecutionStackTelemetry` contains:

```text
Available
MinimumFreeBytes
```

A provider advertising `SupportsStackTelemetry = true` can report the minimum unused stack bytes observed during the native context lifetime.

When telemetry is unavailable, consumers must honor `Available == false` rather than treating zero bytes as a measured result.

## Current-context inspection

`IsCurrentContext()` is the Boolean predicate identifying whether the caller is currently executing inside that provider's native context.

This supports ownership/lifecycle checks such as preventing a context from joining itself without exposing the native task/thread handle.

## Yield

Every provider exposes static:

```cpp
Yield() noexcept
```

This yields the selected native execution runtime. It is only a low-level Platform operation; it does not define fairness, scheduling, cooperative cancellation, or ESPressio Thread semantics.

## Substitution and provider ownership

Concrete providers belong to repositories matching the API that implements the abstraction, for example:

```text
FreeRTOS API -> EDP-Platform-FreeRTOS
ESP-IDF API  -> EDP-Platform-ESP-IDF
```

A consuming build selects the provider through Composition and allocates runtime/provider/storage objects through ordinary C++ Bootstrap ownership.

## Explicit non-goals

`ExecutionContext` does not define:

```text
ESPressio Task or Thread semantics
task pools
persistent loop behavior
precision scheduling
deadlines
cancellation tokens
forced termination
future/promise semantics
dynamic work queues
application scheduler policy
```

Those semantics must be layered above Platform rather than added to this native execution abstraction.
