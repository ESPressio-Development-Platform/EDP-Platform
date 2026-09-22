# EDP-Platform Dependency Graph

> Revalidated: 2026-09-22 — RandomByteSource capability and cryptographic-suitability property introduce no new ESPressio-library dependency.\n\n> Revalidated: 2026-09-20 — Threading-support audit retains only the portable execution-priority contract and introduces no new ESPressio-library dependency.

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
