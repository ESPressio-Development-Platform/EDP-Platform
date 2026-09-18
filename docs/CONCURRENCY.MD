# Concurrency Foundation

## Contents

1. [Purpose](#purpose)
2. [Why This Exists](#why-this-exists)
3. [Dependency Boundary](#dependency-boundary)
4. [Composition Model](#composition-model)
5. [AtomicWord32](#atomicword32)
6. [Memory Ordering Surface](#memory-ordering-surface)
7. [Why 32 Bits](#why-32-bits)
8. [ConcurrentSnapshot](#concurrentsnapshot)
9. [Publication Algorithm](#publication-algorithm)
10. [Reader Algorithm](#reader-algorithm)
11. [Why This Is Not a Classic Seqlock](#why-this-is-not-a-classic-seqlock)
12. [Generation Rollover](#generation-rollover)
13. [Single-Writer Contract](#single-writer-contract)
14. [Memory Model](#memory-model)
15. [Thread and ISR Semantics](#thread-and-isr-semantics)
16. [Composition and Substitution](#composition-and-substitution)
17. [Example](#example)
18. [Synchronized Clock Integration](#synchronized-clock-integration)
19. [What This Tranche Does Not Contain](#what-this-tranche-does-not-contain)

---

## Purpose

The concurrency foundation supplies the smallest Platform-level mechanism currently required by higher ESPressio domains that publish read-mostly state across thread boundaries.

A current consumer is `EDP-Clock`'s synchronized Clock implementation. Its discipline state changes occasionally while `SynchronizedNow()` may be called frequently and concurrently from many execution contexts.

The design therefore optimizes for:

- extremely cheap reads;
- fixed and inspectable memory cost;
- no heap allocation;
- no hidden worker;
- no mutex on the read path;
- no scheduler dependency;
- no Arduino, ESP-IDF, or FreeRTOS dependency;
- coherent whole-state publication rather than individually coherent fields.

---

## Why This Exists

A collection of independently atomic fields is not automatically a coherent state.

For example, a synchronized clock may eventually retain values such as:

```text
monotonic anchor
synchronized anchor
rate correction
phase correction
uncertainty
synchronization state
```

A reader must never accidentally combine:

```text
new monotonic anchor
old synchronized anchor
new uncertainty
old rate correction
```

Every returned observation must derive from one complete published generation.

`ConcurrentSnapshot` provides that guarantee for a fixed, trivially-copyable state object.

---

## Dependency Boundary

The concurrency foundation depends on:

```text
EDP-System
    └── Composition Framework

C++ standard facilities
    ├── fixed-width integers
    ├── fixed-size arrays
    ├── type traits
    └── memcpy for object-representation transfer
```

It does not depend on:

```text
Arduino
ESP-IDF
FreeRTOS
mutexes
semaphores
tasks
queues
critical sections
condition variables
platform interrupt APIs
```

Concrete Platform libraries must provide an `AtomicWord32` implementation that advertises `LockFree = true` and `AtomicWordStorageBytes = 4`. `ConcurrentSnapshot` validates those Composition properties and the required `Word` operations at compile time.

The abstraction library therefore never assumes that a particular SDK, compiler primitive, or operating-system synchronization mechanism exists.

---

## Composition Model

`EDP-Platform` defines one Platform Composition domain:

```cpp
ESPressio::Platform::Domain
```

The first capability within that domain is:

```cpp
ESPressio::Platform::Concurrency::AtomicWord32
```

It is exclusive because one application Composition should select one implementation contract for this primitive.

The capability currently advertises two properties:

```cpp
LockFree
AtomicWordStorageBytes
```

A conforming provider must advertise:

```text
LockFree               = true
AtomicWordStorageBytes = 4
```

`ConcurrentSnapshot` rejects a provider that does not advertise those guarantees.

---

## AtomicWord32

`AtomicWord32` is the Platform capability representing the minimum indivisible storage unit used by the current ESPressio concurrency model.

The abstraction deliberately does **not** define one concrete object type. A selected provider exposes a nested:

```cpp
Provider::Word
```

whose object occupies exactly four bytes and provides the required atomic operations.

Concrete implementations reside outside `EDP-Platform`, for example in:

```text
EDP-Platform-Arduino
EDP-Platform-ESP-IDF
EDP-Platform-FreeRTOS
```

This keeps SDK/toolchain-specific implementation decisions out of the abstraction repository while allowing `ConcurrentSnapshot` to remain fully generic.

---

## Memory Ordering Surface

The ESPressio API deliberately does not expose the complete `std::memory_order` vocabulary.

Instead, the word exposes the operations required by Platform facilities with ordering made explicit in the operation name:

```cpp
LoadRelaxed()
LoadAcquire()
StoreRelaxed(...)
StoreRelease(...)
CompareExchangeAcqRel(...)
```

This keeps consumer code in ESPressio vocabulary while preserving the precise ordering semantics needed by generic concurrency algorithms.

`CompareExchangeAcqRel()` is included as a foundational atomic operation even though `ConcurrentSnapshot` does not need it on its hot read or publish paths.

---

## Why 32 Bits

The platform baseline includes 32-bit MCUs.

Requiring a lock-free 64-bit atomic would unnecessarily restrict otherwise suitable targets and may introduce compiler-runtime helpers or hidden locking.

The foundation therefore requires only:

```text
one naturally supported lock-free 32-bit atomic word
```

Larger logical values are represented using multiple 32-bit atomic words and coherent publication rules.

This is especially important for future Clock state, where timestamps and anchors naturally use 64-bit nanosecond values.

---

## ConcurrentSnapshot

`ConcurrentSnapshot` is a fixed-storage utility:

```cpp
template<
    class TState,
    class TAtomicWordProvider
>
class ConcurrentSnapshot;
```

`TState` must be:

- trivially copyable;
- default constructible.

The state may contain ordinary 8-bit, 16-bit, 32-bit, or 64-bit scalar fields. It does not need to contain atomic members.

The utility stores the state as 32-bit atomic words internally.

### Semantic contract

Each snapshot has:

```text
exactly one writer
zero or more concurrent readers
```

The writer always publishes a complete replacement state.

A reader always receives one complete generation or retries.

No reader can receive a mixture of fields from two successfully published generations.

---

## Publication Algorithm

Two state buffers are retained:

```text
Buffer 0
Buffer 1
```

Only one is considered published at a time.

The low bit of the logical publication generation identifies the active buffer.

When publishing:

```text
1. read current generation
2. calculate next generation
3. choose the buffer that will become active next
4. write every state word into that currently inactive buffer
5. publish the next generation with release ordering
```

The generation change is the publication boundary.

All relaxed writes performed before the release store become visible to a reader that acquires the published generation.

There is no writer-side heap allocation, mutex, semaphore, task interaction, or callback.

---

## Reader Algorithm

A reader performs:

```text
1. acquire current publication generation
2. select the active buffer
3. copy every atomic word using relaxed loads
4. acquire publication generation again
5. if the generation is unchanged, decode and return
6. otherwise retry
```

The second generation read detects a publication that overlapped the copy.

A retry is not a blocked wait on the writer. The reader simply starts again from the newly published state.

---

## Why This Is Not a Classic Seqlock

A classic seqlock commonly marks the shared object as "write in progress", modifies that same object, and makes readers retry while the writer-owned sequence is odd.

That is unattractive for a pre-emptive embedded system.

Consider:

```text
low-priority writer
    marks write in progress
    begins update

high-priority reader
    pre-empts writer
    spins waiting for write to finish
```

If the writer cannot run while the reader spins, the system has created a priority-induced deadlock/livelock condition.

`ConcurrentSnapshot` avoids that structure.

The writer never invalidates the currently published buffer while preparing the next state. Readers may continue consuming the last published state while the writer is pre-empted indefinitely.

Only a completed publication changes the generation.

This property is particularly useful for Clock reads, where obtaining the last coherent mapping is better than depending on a lower-priority synchronization-update task being scheduled.

---

## Generation Rollover

The logical publication generation is 64 bits, but it is represented using two lock-free 32-bit atomic words.

The low word is the release/acquire publication token.

On low-word rollover:

```text
write next state buffer
store new high generation word (relaxed)
store wrapped low generation word (release)
```

A reader acquires the low word before reading the high word. Once the wrapped low value is observed, the preceding high-word update and buffer writes are also visible.

The logical generation therefore does not repeat until `2^64` completed publications.

No lock-free 64-bit atomic operation is required.

---

## Single-Writer Contract

`Publish()` is intentionally a single-writer operation.

This is a semantic contract, not an internal mutex policy.

The design does not spend RAM on writer ownership state because the immediate consumers already have a single authority responsible for producing replacement state.

For synchronized Clock, many observation sources may exist, but one Clock discipline authority owns mutation of the public synchronization mapping.

If an application has multiple producers, it must serialize them before they reach one `ConcurrentSnapshot` writer.

---

## Memory Model

For a state occupying `N` 32-bit words, the snapshot retains:

```text
2 × N atomic words for state buffers
2 atomic words for the 64-bit logical publication generation
```

Therefore the atomic storage requirement is:

```text
((2 × N) + 2) × 4 bytes
```

No heap storage is used.

For example, a 24-byte state occupies six words:

```text
2 × 6 × 4 = 48 bytes   state buffers
2 × 4     =  8 bytes   generation
---------------------
             56 bytes   total atomic storage
```

`ConcurrentSnapshot` exposes:

```cpp
StateBytes
WordsPerBuffer
AtomicStorageBytes
```

so the memory consequence of a proposed state is visible at compile time.

---

## Thread and ISR Semantics

The current contract guarantees concurrent **thread/task** readers when the selected atomic provider satisfies its advertised capability.

`ConcurrentSnapshot` itself does not call an RTOS and does not know which scheduler, if any, is present.

ISR safety is deliberately not claimed by this tranche.

A future concrete provider may be ISR-safe, but that must be an explicit capability/property contract before higher layers depend upon it.

This prevents accidental assumptions that are valid for one MCU/compiler combination but invalid for another.

---

## Composition and Substitution

The abstraction is intentionally substitutable.

Any concrete Platform library may define a provider that supplies:

```cpp
AtomicWord32
```

with:

```text
LockFree = true
AtomicWordStorageBytes = 4
```

and exposes a conforming nested `Word` type.

`ConcurrentSnapshot<TState, TProvider>` uses that selected provider without changing its publication algorithm or consumer-facing API.

This is the compile-time substitution boundary: the higher-level utility depends on the ESPressio capability contract, not on Arduino, ESP-IDF, FreeRTOS, or any other SDK vocabulary.

---

## Example

A complete fixed-storage use looks like:

```cpp
struct SharedState final {
    std::uint32_t Generation;
    std::uint32_t Value;
};

using SelectedAtomicProvider = MyPlatformAtomicWord32Provider;

ESPressio::Platform::Concurrency::ConcurrentSnapshot<
    SharedState,
    SelectedAtomicProvider
> state(
    SharedState {
        0U,
        100U
    }
);

state.Publish(
    SharedState {
        1U,
        200U
    }
);

const auto current = state.Read();
```

The reader sees either the complete initial state or the complete replacement state.

It cannot observe:

```text
Generation = 1
Value      = 100
```

as a successfully validated snapshot.

---

## Synchronized Clock Integration

The current dependency path is:

```text
EDP-System
    ↓
EDP-Platform
    ├── AtomicWord32
    └── ConcurrentSnapshot
    ↓
EDP-Clock
    └── SynchronizedClock
```

The synchronized Clock discipline retains one compact plain state structure describing the current monotonic-to-synchronized mapping and publishes that structure through `ConcurrentSnapshot`.

`SynchronizedNow()` can then:

```text
capture one coherent mapping snapshot
capture/use the monotonic coordinate
project synchronized time
calculate uncertainty/state from that same mapping generation
return SynchronizedReading
```

The Clock implementation therefore does not need to carry a Platform mutex, FreeRTOS semaphore, or OS-specific critical section merely to make reads coherent.

---

## What the Concurrency Subsystem Does Not Own

The concurrency subsystem remains deliberately narrow.

It does not own:

```text
Task or Thread semantics
Mutex / RecursiveMutex / ReadWriteMutex
CountingSemaphore
Signal
SpinLock
Scheduler semantics
Delay / sleep semantics
64-bit atomic capability
heap-backed synchronization objects
```

The lock, semaphore, signal, and spin-lock contracts that are now implemented by `EDP-Platform` belong to the separate [Synchronization](./SYNCHRONIZATION.MD) subsystem rather than to `ConcurrentSnapshot` or `AtomicWord32`.

Higher Task, Thread, pooling, loop, precision-execution, and scheduler semantics remain outside `EDP-Platform` and belong to higher ESPressio domains such as `EDP-Threading`.

`ConcurrentSnapshot` itself still claims only the thread/task concurrency semantics documented above. It does not claim a general ISR-safe publication contract.
