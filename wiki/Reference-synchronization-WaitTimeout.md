# src/synchronization/WaitTimeout.hpp

**Primary classification:** PUBLIC API

**Source baseline:** `7fce256f577b6cab76b6a5a7dcd64688154262bc`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/7fce256f577b6cab76b6a5a7dcd64688154262bc/src/synchronization/WaitTimeout.hpp)

## Direct includes

- `cstdint`

## Documented declarations

### `WaitMode`

**Classification:** PUBLIC API

Identifies how a synchronization wait should behave.

```cpp
enum class WaitMode : std::uint8_t
```

### `WaitTimeout`

**Classification:** PUBLIC API

Platform-neutral relative wait request expressed in nanoseconds.

```cpp
class WaitTimeout final
```

### `_mode`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Selected wait behavior.

```cpp
WaitMode _mode;
```

### `_nanoseconds`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Relative wait duration in nanoseconds when the mode is Finite.

```cpp
std::uint64_t _nanoseconds;
```

### `NoWait`

**Classification:** PUBLIC API · source access: `public`

Creates an immediate non-blocking wait request.

```cpp
static constexpr WaitTimeout NoWait() noexcept
```

### `ForNanoseconds`

**Classification:** PUBLIC API · source access: `public`

Creates a finite relative wait request.

```cpp
static constexpr WaitTimeout ForNanoseconds(
                std::uint64_t nanoseconds
            ) noexcept
```

### `Forever`

**Classification:** PUBLIC API · source access: `public`

Creates an indefinite wait request.

```cpp
static constexpr WaitTimeout Forever() noexcept
```

### `Mode`

**Classification:** PUBLIC API · source access: `public`

Returns the selected wait mode.

```cpp
constexpr WaitMode Mode() const noexcept
```

### `Nanoseconds`

**Classification:** PUBLIC API · source access: `public`

Returns the finite relative duration in nanoseconds, or zero for non-finite modes.

```cpp
constexpr std::uint64_t Nanoseconds() const noexcept
```

### `IsNoWait`

**Classification:** PUBLIC API · source access: `public`

Reports whether the request is non-blocking.

```cpp
constexpr bool IsNoWait() const noexcept
```

### `IsFinite`

**Classification:** PUBLIC API · source access: `public`

Reports whether the request has a finite duration.

```cpp
constexpr bool IsFinite() const noexcept
```

### `IsForever`

**Classification:** PUBLIC API · source access: `public`

Reports whether the request waits indefinitely.

```cpp
constexpr bool IsForever() const noexcept
```

### `WaitTimeout`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Creates a validated wait request from its normalized representation.

```cpp
constexpr WaitTimeout(
                WaitMode mode,
                std::uint64_t nanoseconds
            ) noexcept :
                _mode(mode),
```

