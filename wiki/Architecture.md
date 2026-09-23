# Architecture

The library is deliberately below concrete SDK integrations. It defines AtomicWord32 and ConcurrentSnapshot for coherent low-level publication, synchronization capabilities for mutexes/semaphores/signals/spin locks, ExecutionContext as the native schedulable execution abstraction, and RandomByteSource for caller-buffer random generation.

ExecutionContext is intentionally lower-level than EDP-Threading. It owns native-context lifecycle and required caller-supplied backing storage, but not Task/Thread semantics, scheduling policy or application shutdown semantics.
