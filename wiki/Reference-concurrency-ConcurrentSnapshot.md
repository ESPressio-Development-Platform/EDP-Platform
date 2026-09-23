# src/concurrency/ConcurrentSnapshot.hpp

**Primary classification:** PUBLIC API

**Source baseline:** `34c35862978f9a37c5d99c2198b91134042038eb`

[Open exact source](https://github.com/ESPressio-Development-Platform/EDP-Platform/blob/34c35862978f9a37c5d99c2198b91134042038eb/src/concurrency/ConcurrentSnapshot.hpp)

## Direct includes

- `array`
- `cstddef`
- `cstdint`
- `cstring`
- `type_traits`
- `AtomicWord32Contract.hpp`

## Documented declarations

### `SnapshotGeneration`

**Classification:** PUBLIC API

Coherent publication generation represented by two lock-free 32-bit atomic words.

```cpp
struct SnapshotGeneration final
```

### `High`

**Classification:** PUBLIC API · source access: `public`

High 32 bits of the logical publication generation.

```cpp
std::uint32_t High;
```

### `Low`

**Classification:** PUBLIC API · source access: `public`

Low 32 bits of the logical publication generation.

```cpp
std::uint32_t Low;
```

### `SameGeneration`

**Classification:** PUBLIC API

Determines whether two publication generations identify the same published snapshot.

```cpp
constexpr bool SameGeneration(
            const SnapshotGeneration& left,
            const SnapshotGeneration& right
        ) noexcept
```

### `TState`

**Classification:** PUBLIC API

Fixed-storage single-writer/multi-reader coherent state publication.

Two atomic-word buffers are retained. The single writer always fills the inactive buffer
before publishing a new generation. Readers copy only from the currently published buffer
and validate that the publication generation did not change while the copy was in progress.
A reader may retry when publication overlaps its read, but it never waits for a writer-held
mutex, critical section, task, semaphore, or other blocking synchronization primitive.

- **Template parameter `TState`:** Trivially-copyable state published as one coherent snapshot.
- **Template parameter `TAtomicWordProvider`:** Provider satisfying the AtomicWord32 capability.

```cpp
template<class TState, class TAtomicWordProvider>
    class ConcurrentSnapshot final
```

### `AtomicTraits`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Validated atomic-provider metadata used by this snapshot.

```cpp
using AtomicTraits = Detail::AtomicWord32ProviderTraits<TAtomicWordProvider>;
```

### `Word`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Concrete four-byte atomic word supplied by the selected provider.

```cpp
using Word = typename AtomicTraits::Word;
```

### `sizeof`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Number of atomic words required to store one complete state representation.

```cpp
static constexpr std::size_t WordCount =
            (sizeof(TState) + sizeof(std::uint32_t) - 1U) /
            sizeof(std::uint32_t);
```

### `Buffer`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Atomic-word array holding one complete state representation.

```cpp
using Buffer = std::array<Word, WordCount>;
```

### `PlainBuffer`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Plain-word array used while encoding or decoding one state representation.

```cpp
using PlainBuffer = std::array<std::uint32_t, WordCount>;
```

### `Buffer`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Alternating fixed buffers holding atomic representations of published state.

```cpp
std::array<Buffer, 2U> _buffers;
```

### `_generationHigh`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

High 32 bits of the logical publication generation.

```cpp
Word _generationHigh;
```

### `_generationLow`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Low 32 bits of the logical publication generation and active-buffer selector.

```cpp
Word _generationLow;
```

### `Encode`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Converts a state object into zero-padded 32-bit words without type-punning.

```cpp
static PlainBuffer Encode(
            const TState& state
        ) noexcept
```

### `Decode`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Reconstructs a state object from its 32-bit word representation without type-punning.

```cpp
static TState Decode(
            const PlainBuffer& encoded
        ) noexcept
```

### `WriteBuffer`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Replaces one inactive atomic buffer with the supplied state representation.

```cpp
void WriteBuffer(
            std::size_t bufferIndex,
            const TState& state
        ) noexcept
```

### `ReadBuffer`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Copies one published atomic buffer into a plain state representation.

```cpp
PlainBuffer ReadBuffer(
            std::size_t bufferIndex
        ) const noexcept
```

### `ReadGeneration`

**Classification:** PRIVATE IMPLEMENTATION · source access: `private`

Reads the logical 64-bit publication generation using only lock-free 32-bit operations.

```cpp
Detail::SnapshotGeneration ReadGeneration() const noexcept
```

### `ConcurrentSnapshot`

**Classification:** PUBLIC API · source access: `public`

Creates a snapshot whose initial published state is value-initialized.

```cpp
ConcurrentSnapshot() noexcept :
            ConcurrentSnapshot(TState {}) {}
```

### `ConcurrentSnapshot`

**Classification:** PUBLIC API · source access: `public`

Creates a snapshot with the supplied initial state already coherently published.

- **Parameter `initialState`:** Initial state visible to every reader before the first Publish call.

```cpp
explicit ConcurrentSnapshot(
            const TState& initialState
        ) noexcept :
            _buffers {},
```

### `ConcurrentSnapshot`

**Classification:** PUBLIC API · source access: `public`

Prevents duplicating live concurrency state.

```cpp
ConcurrentSnapshot(const ConcurrentSnapshot&) = delete;
```

### `operator`

**Classification:** PUBLIC API · source access: `public`

Prevents copy assignment of live concurrency state.

```cpp
ConcurrentSnapshot& operator =(const ConcurrentSnapshot&) = delete;
```

### `ConcurrentSnapshot`

**Classification:** PUBLIC API · source access: `public`

Prevents relocating live concurrency state while readers may reference it.

```cpp
ConcurrentSnapshot(ConcurrentSnapshot&&) = delete;
```

### `operator`

**Classification:** PUBLIC API · source access: `public`

Prevents move assignment of live concurrency state.

```cpp
ConcurrentSnapshot& operator =(ConcurrentSnapshot&&) = delete;
```

### `sizeof`

**Classification:** PUBLIC API · source access: `public`

Number of bytes in the consumer state represented by this snapshot.

```cpp
static constexpr std::size_t StateBytes = sizeof(TState);
```

### `WordsPerBuffer`

**Classification:** PUBLIC API · source access: `public`

Number of atomic words used by one state buffer.

```cpp
static constexpr std::size_t WordsPerBuffer = WordCount;
```

### `sizeof`

**Classification:** PUBLIC API · source access: `public`

Number of bytes reserved for the two state buffers and logical publication generation.

```cpp
static constexpr std::size_t AtomicStorageBytes =
            ((2U * WordCount) + 2U) * sizeof(std::uint32_t);
```

### `Publish`

**Classification:** PUBLIC API · source access: `public`

Publishes a complete replacement state for concurrent readers.

Exactly one execution context may call Publish for a given ConcurrentSnapshot object.
The single-writer contract allows publication to remain lock-free without retaining a
writer mutex or compare/exchange ownership word.

- **Parameter `state`:** Complete replacement state to publish.

```cpp
void Publish(
            const TState& state
        ) noexcept
```

### `Read`

**Classification:** PUBLIC API · source access: `public`

Returns one coherent state snapshot to a concurrent reader.

A publication overlapping the copy causes this method to retry from the newly published
buffer. Reader execution never depends on the writer being scheduled or completing a
critical section, so pre-empting a writer cannot deadlock a reader.

```cpp
TState Read() const noexcept
```

