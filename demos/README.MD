# Demos

- `concurrent-snapshot-basic` — coherent fixed-storage publication through `ConcurrentSnapshot`, using the Standard C++ `AtomicWord32` provider from `EDP-Platform-Portable`.
- `synchronization-basic` — basic mutex and latched-signal use through the FreeRTOS concrete providers.
- `execution-context-basic` — caller-supplied control/stack storage used to create, start, join and destroy one native execution context.

Each logical demo provides Arduino IDE, PlatformIO Arduino and PlatformIO ESP-IDF-targeted surfaces.

Concrete providers are demo/build selections only and do not change the `EDP-Platform` dependency graph.

## C++ Language Baseline

All PlatformIO-compatible demos require C++20 and select the maintained PIOArduino ESP32 platform package explicitly. PIOArduino is the recommended ESP32 PlatformIO-compatible environment for ESPressio.
