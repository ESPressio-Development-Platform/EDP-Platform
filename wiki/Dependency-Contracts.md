# Dependency Contracts

EDP-Platform has one mandatory EDP dependency: **EDP-System**.

## EDP-System

EDP-Platform uses the EDP-System Composition Framework to define the Platform domain and its capability/property contracts. There is no runtime System service dependency.

## Contracts exported downstream

### AtomicWord32

Exclusive Platform capability. Providers must advertise `LockFree` and `AtomicWordStorageBytes` and satisfy the operation contract validated by `Concurrency::Detail::AtomicWord32ProviderTraits`.

### ExecutionContext

Exclusive Platform capability. Providers advertise caller-supplied-storage support, priority/affinity/telemetry support, control/stack storage requirements, stack allocation granularity and join-wait resolution. `Execution::Detail::ExecutionContextProviderTraits` is an **internal provider API** consumed by concrete provider repositories and EDP-Threading.

### Synchronization

Mutex, RecursiveMutex, ReadWriteMutex, CountingSemaphore, Signal and SpinLock are independent exclusive capabilities. Wait resolution and interrupt-context support are capability-specific properties. The `Synchronization::Detail::*ProviderTraits` types form an internal provider contract used by concrete provider repositories and higher-level runtimes.

### RandomByteSource

Shared capability. `RandomByteSourceFeatures` is a flag property. `CryptographicRandomByteSourceRequirement` selects at least one provider with the `CryptographicallySuitable` feature.

## Ownership boundary

EDP-Platform defines contracts only. SDK-backed implementations belong to the repository owning the native API: FreeRTOS, ESP-IDF, Portable, or Arduino-specific integration.

> Dependency contract audit baseline: `785070b8c408627eb54eab495214a75f61f9bb75` (`main`).
