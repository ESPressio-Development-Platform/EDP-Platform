# Platform Execution Context

## Purpose

`ExecutionContext` is the lowest ESPressio abstraction for one native schedulable execution context.

It deliberately does not define ESPressio Task, Thread, loop, future, pool or precision semantics.

Those higher semantics belong to `EDP-Threading`.

## Static storage

The baseline contract is deterministic.

A provider must use caller-supplied native control storage and caller-supplied stack storage.

Providers advertise:

```text
ControlStorageBytes
ControlStorageAlignment
StackStorageAlignment
StackAllocationGranularityBytes
```

The provider object itself may retain small fixed control state, but it must not hide task-stack or native task-control allocation behind the Platform API.

## Lifecycle

The portable lifecycle is:

```text
Initialize
Start
Join
Destroy
```

`Initialize` binds the supplied storage, entry function, parameter and configuration.

`Start` makes the initialized context eligible to execute.

The entry function returns when its higher-level execution semantics are complete.

`Join` waits for that completion.

`Destroy` retires the native context and releases its claim on caller-supplied storage.

## Scheduling options

Priority, processor affinity and stack telemetry are provider capabilities.

A provider advertises whether each optional feature is supported.

A consumer requiring one of those features must enforce that property through Composition.

## Naming

The Platform contract is intentionally called `ExecutionContext` rather than Task, Thread or HardwareThread.

`Task` and `Thread` have ESPressio semantic meanings that belong to `EDP-Threading`.

An RTOS task is also not necessarily a hardware thread.

## No forced termination semantics

The Platform execution contract does not impose a Threading-level stop or cancellation policy.

Higher layers must arrange cooperative termination and allow the entry function to return before normal join/destruction.
