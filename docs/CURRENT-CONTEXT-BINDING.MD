# Current-context binding

An optional Platform capability carries one typed, caller-owned record for the current ordinary execution context. It supplies no context pool, identity allocator, clock, scheduler, Profiling model or Threading dependency. Existing ExecutionContext contracts and defaults remain unchanged.

Include ESPressio_Platform_CurrentContextBinding.hpp for only this vocabulary. Select exactly one concrete provider and record Type in application System Composition; pass it explicitly to its lifecycle owner and consumers. The native realization is in [EDP-Platform-ESP-IDF](https://github.com/ESPressio-Development-Platform/EDP-Platform-ESP-IDF). No concrete provider becomes a dependency of Platform.

| Operation | Exact result | Behavior |
|---|---|---|
| BindCurrent(const ContextRecord&) | CurrentContextBindResult | Bound only when empty; AlreadyBound preserves the existing record; UnsupportedContext changes nothing. |
| ReadCurrent() const | CurrentContextReadResult<ContextRecord> | Bound carries a borrowed pointer; Unbound and UnsupportedContext carry nullptr. |
| RetireCurrent(const ContextRecord&) | CurrentContextRetireResult | Retired clears only the expected address; NotBound, DifferentBinding and UnsupportedContext preserve state. |

All operations are noexcept. The optional trait requires the exclusive CurrentContextBinding capability and true ConstantTimeLookup, AllocationFreeLookup, CurrentContextOnly and RejectsInterruptContexts properties. It checks exact result Types, record Type and noncopyable/nonmovable provider lifetime. Metadata cannot prove target runtime behavior; each concrete provider needs qualification.

## Progressive integration

1. Define the application's record Type. The owner supplies any qualified identity and direct slot mapping; the carrier does not allocate or validate them.

~~~cpp
struct ContextRecord final {
    std::uint64_t identity;
    std::uint32_t slot;
};
~~~

2. At the ordinary execution owner's actual entry, bind an address-stable record before eligible captured work.

~~~cpp
const auto result = provider.BindCurrent(record);
if (result != ESPressio::Platform::Execution::CurrentContextBindResult::Bound) {
    return;
}
~~~

3. Read a typed borrow in bounded work.

~~~cpp
const auto current = provider.ReadCurrent();
if (const auto* record = current.GetIfBound()) {
    UseOwnerRecord(*record);
}
~~~

4. After the final captured exit, retire the exact record and handle its typed outcome.

~~~cpp
const auto retired = provider.RetireCurrent(record);
~~~

The record must outlive its binding and every outstanding read. A copied ReadResult does not extend lifetime. Its factories prevent borrowing immediate temporary records but cannot prove owner lifetime. Retirement does not wait for borrows, clear another context, erase an activity flag, drain a recorder or prove durable completion. Abnormal task destruction requires independent owner proof of non-resumption.

Interrupts are unsupported; rejection prevents attribution to an interrupted task. This is not an ISR-safety promise. Follow project ISR policy.

## Qualification

The maintained contract runner checks one valid fixture and ten independently invalid shapes, including missing/false metadata, wrong exact results/record Type, throwing methods and copied provider identity. Run the complete existing Platform CTest suite too.

Native qualification additionally requires compiler TLS initialization, cross-translation-unit identity, task switches, reuse, ISR rejection, actual stack cost and Disabled symbol elimination. Host fixtures cannot establish these native properties.

[Exact source reference](../wiki/Reference-execution-CurrentContextBinding.md). [Typed-result demo](../demos/current-context-binding-results/README.MD).
