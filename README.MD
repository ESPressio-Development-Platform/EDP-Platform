# ESPressio Platform

`EDP-Platform` defines Platform-level abstractions and platform-neutral foundational utilities consumed by higher ESPressio domains.

It contains **contracts, not platform implementations**.

Arduino, ESP-IDF, FreeRTOS, Standard C++ and other concrete implementations belong in their corresponding `EDP-Platform-*` repositories and are selected by application Composition.

## Architectural Dependency Rule

**`EDP-Platform` is a foundational abstraction layer. Its only permitted ESPressio dependency is `EDP-System`, used solely for the Composition Framework.**

`EDP-Platform` must never depend on another ESPressio domain/library.

The dependency direction is always:

```text
EDP-System
    ↓
EDP-Platform
    ↓
higher ESPressio domains
```

and never the reverse.

If a proposed Platform abstraction appears to require `EDP-Memory`, `EDP-Clock`, `EDP-Threading`, `EDP-Serializable` or another higher ESPressio library, the abstraction must be redesigned in Platform-owned or Standard C++ vocabulary rather than adding that dependency.

## Contents

The following Platform facilities are currently provided:

- [Concurrency Foundation](./docs/CONCURRENCY.MD) — lock-free 32-bit atomic-word capability and fixed-storage `ConcurrentSnapshot`.
- [Synchronization](./docs/SYNCHRONIZATION.MD) — mutex, recursive mutex, read/write mutex, counting semaphore, latched signal and spin-lock capability contracts.
- [Execution Context](./docs/EXECUTION.MD) — caller-storage-backed native execution-context capability used by higher execution semantics.

## Dependencies

```text
EDP-Platform
└── EDP-System [mandatory]
```

`EDP-System` is required only for the Composition Framework.

`EDP-Platform` has no optional ESPressio library dependencies.

Concrete Platform providers are selected by consuming application Composition and are not dependencies of this abstraction library.

See [DEPENDENCY_GRAPH.MD](./DEPENDENCY_GRAPH.MD) for the maintained dependency graph.

## Concrete Provider Ownership

Concrete ownership follows the API that implements the abstraction:

```text
Arduino API     -> EDP-Platform-Arduino
ESP-IDF API     -> EDP-Platform-ESP-IDF
FreeRTOS API    -> EDP-Platform-FreeRTOS
Standard C++    -> EDP-Platform-Portable
```

A framework embedding another runtime does not transfer ownership. For example, a FreeRTOS-backed mutex used by an Arduino-ESP32 application is still provided by `EDP-Platform-FreeRTOS`.

## Demos

Buildable demonstrations remain under `./demos` for Arduino IDE, PlatformIO Arduino and PlatformIO ESP-IDF targets as required by ESPressio policy.

Demo/build provider dependencies do not alter the library dependency graph.

## Development Status

`EDP-Platform` is rebuilt progressively from narrow contracts justified by concrete consumers.

The abstraction library does not pre-emptively accumulate SDK features.

## C++ Language Baseline

ESPressio requires C++20. For PlatformIO-compatible ESP32 development, PIOArduino is the recommended platform baseline because it tracks a current Arduino-ESP32/ESP-IDF toolchain. Stock PlatformIO Core/IDE may still be used when projects explicitly select the PIOArduino platform package and compile as C++20.

See the EDP-System C++20 toolchain documentation for the canonical configuration.
