# ESPressio Platform

`EDP-Platform` defines platform-level abstractions and platform-neutral foundational utilities used by higher ESPressio domains without embedding Arduino, ESP-IDF, FreeRTOS, or other SDK-specific code into those domains.

## Contents

The following Platform facilities are currently provided:

- [Concurrency Foundation](./docs/CONCURRENCY.MD)  
  Defines the lock-free 32-bit atomic-word capability contract and the fixed-storage single-writer/multi-reader `ConcurrentSnapshot` utility required for coherent read-mostly state publication.

## Dependencies

`EDP-Platform` depends only on `EDP-System` for the Composition Framework and standard C++ facilities needed by its platform-neutral utilities.

It contains no Arduino, ESP-IDF, FreeRTOS, operating-system, task, semaphore, mutex, or platform critical-section implementation. Concrete providers satisfying Platform capabilities belong in the corresponding concrete Platform repositories.

## Development Status

`EDP-Platform` is being rebuilt progressively from clean, narrowly-scoped contracts. Platform capabilities are added only when a consuming ESPressio domain has a concrete requirement for them.
