# EDP-Platform Dependency Graph

> Revalidated: 2026-09-23 — Composition Framework enhancement migration changes compile-time provider/requirement metadata and validation only; no ESPressio-library dependency edge changed.

> Revalidated: 2026-09-22 — RandomByteSource capability and cryptographic-suitability property introduce no new ESPressio-library dependency.

> Revalidated: 2026-09-20 — Threading-support audit retains only the portable execution-priority contract and introduces no new ESPressio-library dependency.

## Library Dependency Tree

```text
EDP-Platform
└── EDP-System [mandatory]
```

## Mandatory Dependencies

- `EDP-System` — provides the Composition Framework used to declare Platform domains, capabilities, properties, providers and requirements.

## Optional Dependencies

`EDP-Platform` has no optional ESPressio library dependencies.

This is an architectural invariant.

Concrete Platform repositories satisfy `EDP-Platform` capabilities and higher ESPressio domains consume them. Neither relationship permits `EDP-Platform` to depend upward on those libraries.

Demo projects may select concrete provider libraries as build-only dependencies; those demo dependencies do not alter this library dependency graph.


## H471 coherent snapshot publication

[Buffer-reuse ordering and rollover](docs/H471-SNAPSHOT-ORDERING-PROOF.MD) use existing release/acquire words and the same fixed buffers/metadata. A stable rollover marker preserves reader progress while a writer is paused; the documented logical generation is63bits. No public API, retained storage, Clock, backend, lock, task, dependency or0.1.0 version change. Complete mandatory graph remains EDP-Platform -> EDP-System, with no optional edge.
