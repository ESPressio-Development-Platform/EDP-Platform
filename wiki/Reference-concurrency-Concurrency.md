# src/concurrency/Concurrency.hpp

**Primary classification:** PUBLIC API

**Source:** [current source](../src/concurrency/Concurrency.hpp)


## Direct includes

- `ConcurrencyComposition.hpp`
- `AtomicWord32Contract.hpp`
- `ConcurrentSnapshot.hpp`

## Documented declarations

This header contains no declaration-level Doxygen blocks.



## Optional strong-ordering extension

PUBLIC COMPOSITION property SupportsSequentiallyConsistentLoadStore belongs to AtomicWord32. True promises a single sequentially consistent order across participating strong operations on the provider's words. Absent/false leaves the original contract. The concurrency umbrella also exposes the optional [provider validator](Reference-concurrency-SequentiallyConsistentAtomicWord32Contract); it adds no mandatory methods to base providers. See [normative semantics](../docs/SEQUENTIALLY-CONSISTENT-ATOMIC-WORD.MD).
