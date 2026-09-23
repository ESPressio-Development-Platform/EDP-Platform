# src/execution/ExecutionComposition.hpp

**Primary classification:** PUBLIC COMPOSITION API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/execution/ExecutionComposition.hpp)

## Direct includes

- `cstddef`
- `cstdint`
- `../platform/PlatformComposition.hpp`

## Documented declarations

### `ExecutionContext`

**Classification:** PUBLIC COMPOSITION API

Exclusive Platform capability supplying one native schedulable execution-context implementation.

```cpp
struct ExecutionContext final : Framework::ExclusiveCapability<Platform::Domain> {};
```

### `CallerSuppliedStorage`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether an ExecutionContext provider uses only caller-supplied task/control storage.

```cpp
struct CallerSuppliedStorage final : Framework::Property<ExecutionContext, bool> {};
```

### `SupportsPriority`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether an ExecutionContext provider supports explicit priority configuration.

```cpp
struct SupportsPriority final : Framework::Property<ExecutionContext, bool> {};
```

### `SupportsProcessorAffinity`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether an ExecutionContext provider supports processor-affinity requests.

```cpp
struct SupportsProcessorAffinity final : Framework::Property<ExecutionContext, bool> {};
```

### `SupportsStackTelemetry`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Indicates whether an ExecutionContext provider can report minimum remaining stack bytes.

```cpp
struct SupportsStackTelemetry final : Framework::Property<ExecutionContext, bool> {};
```

### `ControlStorageBytes`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Number of caller-supplied bytes required for native execution-control storage.

```cpp
struct ControlStorageBytes final : Framework::Property<ExecutionContext, std::size_t> {};
```

### `ControlStorageAlignment`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Required alignment of caller-supplied native execution-control storage.

```cpp
struct ControlStorageAlignment final : Framework::Property<ExecutionContext, std::size_t> {};
```

### `StackStorageAlignment`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Required alignment of caller-supplied execution-stack storage.

```cpp
struct StackStorageAlignment final : Framework::Property<ExecutionContext, std::size_t> {};
```

### `StackAllocationGranularityBytes`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native stack allocation granularity in bytes.

```cpp
struct StackAllocationGranularityBytes final : Framework::Property<ExecutionContext, std::size_t> {};
```

### `JoinWaitResolutionNanoseconds`

**Classification:** PUBLIC COMPOSITION API · source access: `public`

Native wait resolution used by execution-context join operations.

```cpp
struct JoinWaitResolutionNanoseconds final : Framework::Property<ExecutionContext, std::uint64_t> {};
```

