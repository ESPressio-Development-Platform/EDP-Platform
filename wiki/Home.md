# EDP-Platform Developer Wiki

EDP-Platform is the SDK-neutral abstraction layer for low-level concurrency, synchronization, execution contexts and randomness. It defines contracts; concrete platform repositories implement them.

This Wiki is maintained beside the code on `main`. Source code and repository `docs/` remain the normative implementation and durable contract sources; the Wiki is the internal developer navigation and explanation layer.

## Public entry point

```cpp
#include <ESPressio_Platform.hpp>
```

## Dependencies

Mandatory: EDP-System only.

## Developer map

Use [Architecture](Architecture.md), [Public API](Public-API.md), [Internal API](Internal-API.md), [Implementation](Implementation.md), [Composition](Composition.md), [Resources / Lifecycle / Concurrency](Resources-Lifecycle-Concurrency.md), and [Build / Test / Source](Build-Test-Source.md).
