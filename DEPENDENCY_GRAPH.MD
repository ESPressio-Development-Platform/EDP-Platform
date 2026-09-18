# EDP-Platform Dependency Graph

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
