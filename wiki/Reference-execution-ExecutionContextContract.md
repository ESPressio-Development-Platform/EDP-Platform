# src/execution/ExecutionContextContract.hpp

**Primary classification:** INTERNAL PROVIDER API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/execution/ExecutionContextContract.hpp)

## Direct includes

- `type_traits`
- `utility`
- `../synchronization/WaitTimeout.hpp`
- `ExecutionComposition.hpp`
- `ExecutionTypes.hpp`

## Documented declarations

### `TExecutionContextProvider`

**Classification:** INTERNAL PROVIDER API

Validates the complete compile-time contract required from an ExecutionContext provider.
- **Template parameter `TExecutionContextProvider`:** Concrete provider Type being validated against the ExecutionContext capability contract.

```cpp
template<class TExecutionContextProvider>
    struct ExecutionContextProviderTraits
```

### `Properties`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Properties advertised for the ExecutionContext capability.

```cpp
using Properties = typename TExecutionContextProvider::CompositionOffers::template PropertiesFor<ExecutionContext>;
```

### `InitializeResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by execution-context initialization.

```cpp
using InitializeResult = decltype(
            std::declval<TExecutionContextProvider&>().Initialize(
                std::declval<const ExecutionStorage&>(),
                std::declval<const ExecutionConfiguration&>(),
                std::declval<ExecutionEntry>(),
                std::declval<void*>()
            )
        );
```

### `StartResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by execution-context start.

```cpp
using StartResult = decltype(
            std::declval<TExecutionContextProvider&>().Start()
        );
```

### `JoinResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by execution-context join.

```cpp
using JoinResult = decltype(
            std::declval<TExecutionContextProvider&>().Join(
                std::declval<Synchronization::WaitTimeout>()
            )
        );
```

### `DestroyResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by execution-context destruction.

```cpp
using DestroyResult = decltype(
            std::declval<TExecutionContextProvider&>().Destroy()
        );
```

### `CurrentContextResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by current-context identity inspection.

```cpp
using CurrentContextResult = decltype(
            std::declval<const TExecutionContextProvider&>().IsCurrentContext()
        );
```

### `StackTelemetryResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by stack telemetry inspection.

```cpp
using StackTelemetryResult = decltype(
            std::declval<const TExecutionContextProvider&>().GetStackTelemetry()
        );
```

### `YieldResult`

**Classification:** INTERNAL PROVIDER API · source access: `public`

Return type produced by yielding the selected execution runtime.

```cpp
using YieldResult = decltype(
            TExecutionContextProvider::Yield()
        );
```

