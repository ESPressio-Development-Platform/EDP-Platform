# Public API

Public capability contracts include AtomicWord32, Mutex, RecursiveMutex, ReadWriteMutex, CountingSemaphore, Signal, SpinLock, ExecutionContext and RandomByteSource.

`WaitTimeout` explicitly distinguishes no-wait, finite nanosecond waits and forever. ExecutionContext exposes logical priority and processor-affinity vocabulary plus provider properties describing support and required storage. RandomByteSource advertises whether output is cryptographically suitable.

Exact declarations, template parameters and signatures remain authoritative in the headers exported by `ESPressio_Platform.hpp`.
