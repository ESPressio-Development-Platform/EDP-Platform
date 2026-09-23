# Private Implementation

ConcurrentSnapshot uses double-buffered state plus a 64-bit generation represented by two 32-bit atomic words. The design avoids classic seqlock reader priority inversion by keeping the published buffer immutable while the single writer prepares the inactive buffer.

ExecutionContext never assumes provider-owned stack/control allocation: backing requirements are part of the contract. Forced termination is deliberately excluded.
