# Random Byte Source

## Purpose

The Platform Randomness facility defines the ESPressio-language contract for providers that fill caller-owned storage with unpredictable bytes.

It exists so higher domains such as Security can request randomness without depending directly on Arduino, ESP-IDF, operating-system, hardware-RNG, or third-party cryptographic APIs.

## Composition capability

`RandomByteSource` is a Shared Platform capability. An application may therefore include multiple independent random-byte providers.

Every provider advertises `RandomByteSourceFeatures`. The currently defined feature is:

- `CryptographicallySuitable` — the provider is suitable for security-sensitive unpredictable-byte generation according to the guarantees documented by its concrete implementation.

Standalone consumers requiring cryptographic randomness use `CryptographicRandomByteSourceRequirement`. Provider Contracts in other Domains reuse `CryptographicRandomByteSourceConstraint` inside an explicitly `ExternalDomain` Requirement, so qualification and dependency scope remain separate.

The feature vocabulary is owned by Platform. Higher domains must not introduce reverse dependencies merely to describe suitability.

## Operation contract

A provider exposes:

```cpp
RandomByteGenerationResult FillRandomBytes(
    std::uint8_t* destination,
    std::size_t byteCount
) noexcept;
```

The caller owns the destination storage.

- A zero byte count succeeds and does not access `destination`.
- A non-zero byte count requires a non-null destination.
- The provider allocates no output buffer.
- Failure is represented by `RandomByteGenerationResult`; Boolean operational status is not used.

## Ownership

`EDP-Platform` defines only the contract.

Concrete ownership follows the API actually used:

- ESP-IDF random APIs -> `EDP-Platform-ESP-IDF`;
- Arduino-specific random APIs -> `EDP-Platform-Arduino` only where the implementation is genuinely Arduino-owned;
- portable Standard C/C++ facilities -> `EDP-Platform-Portable` only when they can satisfy the advertised contract.

## Determinism

The contract performs no allocation. The caller supplies the exact writable byte range, and the provider fills only that range.

Higher domains may choose their own bounded storage and lifecycle policy independently from the random-byte provider.
