# EDP-Platform Dependency Graph

## Library Dependency Tree

```text
EDP-Platform
└── EDP-System [mandatory]
```

## Mandatory Dependencies

- `EDP-System` — provides the Composition Framework used to declare Platform domains, capabilities, properties, providers, and requirements.

## Optional Dependencies

`EDP-Platform` currently has no optional ESPressio library dependencies.

Concrete Platform implementations such as `EDP-Platform-Arduino` and `EDP-Platform-ESP-IDF` satisfy Platform capabilities for consuming applications; they are not dependencies of the abstraction library itself.

The demo projects select target-appropriate concrete providers only as demo/build dependencies.
