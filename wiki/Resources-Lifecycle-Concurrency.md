# Resources, Lifecycle and Concurrency

Resource ownership is explicit. AtomicWord32 is exactly four bytes at the contract level. ConcurrentSnapshot uses fixed double-buffered storage. ExecutionContext requires caller-supplied control and stack backing and advertises exact size/alignment/granularity requirements. ISR support is operation-specific and must be advertised; it is never inferred globally.

## Optional current-context binding

No binding runtime/TLS storage is owned by the Platform vocabulary. Caller records must outlive bindings and every outstanding read; retirement does not wait for borrows or prove task/capture/export completion. Interrupts are unsupported and must be rejected by concrete providers without attributing interrupted task identity. No ISR-safety promise is made.

See [exact reference](Reference-execution-CurrentContextBinding).
