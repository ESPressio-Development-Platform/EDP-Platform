# Optional sequential atomic provider contract

**Classification:** INTERNAL PROVIDER CONTRACT. [Source](../src/concurrency/SequentiallyConsistentAtomicWord32Contract.hpp). [Normative behavior](../docs/SEQUENTIALLY-CONSISTENT-ATOMIC-WORD.MD).

`SequentiallyConsistentAtomicWord32ProviderTraits<TAtomicWordProvider>` completely validates the original provider plus the opt-in stronger contract. TAtomicWordProvider is the concrete Composition-selected provider; its promise applies across participating words in one ordering domain.

| Member | Purpose |
|---|---|
| Base | Preserves all original capability, lock-free, storage, lifetime and operation checks. |
| Properties | Reads actual AtomicWord32 offer metadata; absent/false strong support is rejected. |
| Word | Reuses the base four-byte noncopyable/nonmovable storage type. |
| SequentialLoadResult | Requires the const strong load to return exactly uint32_t. |
| SequentialStoreResult | Requires the infallible strong store to return void. |

Both methods must be noexcept. The trait owns no runtime state/resources and does no synchronization. A complete-type instantiation, such as sizeof, triggers validation. Shape checks do not prove target semantics or finite instruction bounds.

Provider and consumer must share one qualified ordering domain; caller-owned words outlive access, with concrete placement/interrupt constraints. The header includes only the existing owner contract and adds no SDK/higher-domain dependency or production switch. Test-only negative modes and actual validation are documented in the normative document.


## Qualification tooling

tests/run-sequential-atomic-contract-tests.py is an INTERNAL TEST RUNNER. Its main function requires --compiler and --system, uses the real selected owner/System headers, compiles and executes case 0, and verifies both failure and the expected diagnostic for cases 1–8. Temporary executables are removed by TemporaryDirectory; no tracked output is retained. It does not provide a production compiler option or validate a concrete target's ordering semantics.
