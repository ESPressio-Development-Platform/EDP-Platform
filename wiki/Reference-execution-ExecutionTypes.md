# src/execution/ExecutionTypes.hpp

**Primary classification:** PUBLIC API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/execution/ExecutionTypes.hpp)

## Direct includes

- `cstddef`
- `cstdint`

## Documented declarations

### `ExecutionPriority`

**Classification:** PUBLIC API

Portable scheduler-preference levels for one execution context.

```cpp
enum class ExecutionPriority : std::uint8_t
```

### `ProcessorAffinityMode`

**Classification:** PUBLIC API

Selects whether an execution context may run on any processor or one requested processor.

```cpp
enum class ProcessorAffinityMode : std::uint8_t
```

### `ExecutionInitializationResult`

**Classification:** PUBLIC API

Outcome from initializing one execution context.

```cpp
enum class ExecutionInitializationResult : std::uint8_t
```

### `ExecutionStartResult`

**Classification:** PUBLIC API

Outcome from starting one initialized execution context.

```cpp
enum class ExecutionStartResult : std::uint8_t
```

### `ExecutionJoinResult`

**Classification:** PUBLIC API

Outcome from joining one execution context.

```cpp
enum class ExecutionJoinResult : std::uint8_t
```

### `ExecutionDestroyResult`

**Classification:** PUBLIC API

Outcome from destroying one execution context.

```cpp
enum class ExecutionDestroyResult : std::uint8_t
```

### `ProcessorAffinity`

**Classification:** PUBLIC API

Processor-affinity request using ESPressio vocabulary.

```cpp
class ProcessorAffinity final
```

### `_mode`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Selected affinity mode.

```cpp
ProcessorAffinityMode _mode;
```

### `_processorIndex`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Zero-based processor index when the mode is Specific.

```cpp
std::uint32_t _processorIndex;
```

### `ProcessorAffinity`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Creates a normalized processor-affinity request.

```cpp
constexpr ProcessorAffinity(
                ProcessorAffinityMode mode,
                std::uint32_t processorIndex
            ) noexcept :
                _mode(mode),
```

### `Any`

**Classification:** PUBLIC API · source access: `public`

Requests that the provider select any suitable processor.

```cpp
static constexpr ProcessorAffinity Any() noexcept
```

### `Specific`

**Classification:** PUBLIC API · source access: `public`

Requests one specific zero-based processor index.

```cpp
static constexpr ProcessorAffinity Specific(
                std::uint32_t processorIndex
            ) noexcept
```

### `Mode`

**Classification:** PUBLIC API · source access: `public`

Returns the selected affinity mode.

```cpp
constexpr ProcessorAffinityMode Mode() const noexcept
```

### `ProcessorIndex`

**Classification:** PUBLIC API · source access: `public`

Returns the requested processor index.

```cpp
constexpr std::uint32_t ProcessorIndex() const noexcept
```

### `IsAny`

**Classification:** PUBLIC API · source access: `public`

Reports whether any processor is acceptable.

```cpp
constexpr bool IsAny() const noexcept
```

### `ExecutionStorage`

**Classification:** PUBLIC API

Caller-owned storage supplied to one native execution context.

```cpp
struct ExecutionStorage final
```

### `ControlAddress`

**Classification:** PUBLIC API · source access: `public`

Address of caller-owned native control storage.

```cpp
void* ControlAddress = nullptr;
```

### `ControlBytes`

**Classification:** PUBLIC API · source access: `public`

Number of bytes available at ControlAddress.

```cpp
std::size_t ControlBytes = 0U;
```

### `StackAddress`

**Classification:** PUBLIC API · source access: `public`

Address of caller-owned execution-stack storage.

```cpp
void* StackAddress = nullptr;
```

### `StackBytes`

**Classification:** PUBLIC API · source access: `public`

Number of bytes available at StackAddress.

```cpp
std::size_t StackBytes = 0U;
```

### `ExecutionConfiguration`

**Classification:** PUBLIC API

Platform-neutral creation settings for one execution context.

```cpp
struct ExecutionConfiguration final
```

### `Priority`

**Classification:** PUBLIC API · source access: `public`

Portable scheduler-preference level mapped by the concrete provider.

```cpp
ExecutionPriority Priority = ExecutionPriority::Normal;
```

### `Any`

**Classification:** PUBLIC API · source access: `public`

Requested processor affinity.

```cpp
ProcessorAffinity Affinity = ProcessorAffinity::Any();
```

### `Name`

**Classification:** PUBLIC API · source access: `public`

Optional diagnostic name; providers may copy or ignore it.

```cpp
const char* Name = nullptr;
```

### `ExecutionStackTelemetry`

**Classification:** PUBLIC API

Portable stack-use evidence returned by an execution provider.

```cpp
struct ExecutionStackTelemetry final
```

### `Available`

**Classification:** PUBLIC API · source access: `public`

Indicates whether this provider can report the value for this context.

```cpp
bool Available = false;
```

### `MinimumFreeBytes`

**Classification:** PUBLIC API · source access: `public`

Minimum number of stack bytes that remained unused during the context lifetime.

```cpp
std::uint32_t MinimumFreeBytes = 0U;
```

### `ExecutionEntry`

**Classification:** PUBLIC API

Native execution entry function boundary.

```cpp
using ExecutionEntry = void (*)(
        void*
    ) noexcept;
```

