# Resources, Lifecycle and Concurrency

Resource ownership is explicit. AtomicWord32 is exactly four bytes at the contract level. ConcurrentSnapshot uses fixed double-buffered storage. ExecutionContext requires caller-supplied control and stack backing and advertises exact size/alignment/granularity requirements. ISR support is operation-specific and must be advertised; it is never inferred globally.
