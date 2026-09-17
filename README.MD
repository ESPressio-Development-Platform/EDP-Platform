# ESPressio Platform

`EDP-Platform` defines platform-level abstractions and platform-neutral foundational utilities used by higher ESPressio domains without embedding Arduino, ESP-IDF, FreeRTOS, or other SDK-specific code into those domains.

## Contents

The following Platform facilities are currently provided:

- [Concurrency Foundation](./docs/CONCURRENCY.MD)  
  Defines the lock-free 32-bit atomic-word capability contract and the fixed-storage single-writer/multi-reader `ConcurrentSnapshot` utility required for coherent read-mostly state publication.

## Dependencies

```text
EDP-Platform
└── EDP-System [mandatory]
```

`EDP-Platform` currently has no optional ESPressio library dependencies.

Concrete Platform providers are selected by consuming application Composition and are not dependencies of this abstraction library.

See [DEPENDENCY_GRAPH.MD](./DEPENDENCY_GRAPH.MD) for the maintained dependency graph, dependency classification, and demo-only provider notes.

## Demos

A buildable `ConcurrentSnapshot` demonstration is provided under [`./demos/concurrent-snapshot-basic`](./demos/concurrent-snapshot-basic) for:

- Arduino IDE;
- PlatformIO Arduino;
- PlatformIO ESP-IDF.

The Arduino targets use `EDP-Platform-Arduino`; the ESP-IDF target uses `EDP-Platform-ESP-IDF`. Those are demo/build selections only and do not alter the library dependency graph.

## Development Status

`EDP-Platform` is being rebuilt progressively from clean, narrowly-scoped contracts. Platform capabilities are added only when a consuming ESPressio domain has a concrete requirement for them.
