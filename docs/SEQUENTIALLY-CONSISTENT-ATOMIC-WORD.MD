# Sequentially consistent atomic-word ordering

The optional `SupportsSequentiallyConsistentLoadStore` property extends the existing `AtomicWord32` capability. Absence or false preserves the original provider contract. A true offer additionally supplies `LoadSequentiallyConsistent() const noexcept` returning uint32_t and `StoreSequentiallyConsistent(uint32_t) noexcept` returning void.

All participating strong operations across that provider's words share one sequentially consistent ordering domain. Do not substitute acquire/release operations on separate words or mix providers in a cross-word proof. Strong ordering does not repair a racy lifetime or a reused context identity.

## Basic use

Select a concrete provider through application Composition:

```cpp
#include <ESPressio_Platform.hpp>
#include <ESPressio_Platform_Portable_AtomicWord32.hpp>

using Provider = ESPressio::Platform::Portable::Concurrency::AtomicWord32Provider;
using Contract =
    ESPressio::Platform::Concurrency::Detail::SequentiallyConsistentAtomicWord32ProviderTraits<Provider>;
static_assert(sizeof(Contract) > 0U);

Provider::Word published;
published.StoreSequentiallyConsistent(42U);
const auto value = published.LoadSequentiallyConsistent();
```

The concrete provider is an application/demo selection, never a dependency of Platform. A Composition requirement can constrain the SupportsSequentiallyConsistentLoadStore property to true; the complete trait must still validate method shape. Matching AtomicWord32 without that constraint does not require the stronger behavior.

The three concurrent-snapshot demo forms demonstrate this extension alongside the original base-only snapshot behavior. Their feature-branch GitHub dependencies select the matching Platform and Portable implementations.

## Lifetime, resources and qualification

Words retain the base noncopyable/nonmovable four-byte identity. Initialize address-stable storage before publication and retain it through the last potential access. Concrete providers own additional placement and interrupt constraints. The operations are infallible under those preconditions.

Traits validate metadata, base word shape, exact result types and noexcept declarations; they cannot prove native ordering. LockFree does not establish a hard wall-clock bound or qualify every memory region. Validate actual generated code and behavior before selecting a provider for a bounded capture path.

This owner supplies no scheduler, clock, TLS slot, reclamation algorithm, capture gateway or storage durability. Profiling's approved lifecycle is a future consumer, not an implementation in this library.

## Tests and configuration

The maintained contract matrix checks base-only compatibility, a positive optional provider and eight targeted failures: absent/false property, wrong load/store type, throwing load/store and missing load/store. The real Portable provider supplies a separate cross-word ordering regression.

No production compiler switch is added. EDP_PLATFORM_SC_NEGATIVE_CASE is test-only: 0 selects the positive fixture, 1–8 select the corresponding deliberate violation. Original base consumers need no new methods or metadata.
