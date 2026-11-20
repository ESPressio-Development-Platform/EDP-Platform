# Current-context binding reference

Production vocabulary; not an application registry or scheduling implementation.

| Source | Symbols and role |
|---|---|
| [CurrentContextBindingComposition.hpp](../src/execution/CurrentContextBindingComposition.hpp) | CurrentContextBinding exclusive capability; ConstantTimeLookup, AllocationFreeLookup, CurrentContextOnly, RejectsInterruptContexts required true properties. |
| [CurrentContextBindingTypes.hpp](../src/execution/CurrentContextBindingTypes.hpp) | CurrentContextBindResult: Bound, AlreadyBound, UnsupportedContext. CurrentContextReadStatus: Bound, Unbound, UnsupportedContext. CurrentContextRetireResult: Retired, NotBound, DifferentBinding, UnsupportedContext. Every enumerator has an explicit uint8 value. |
| [CurrentContextBindingTypes.hpp](../src/execution/CurrentContextBindingTypes.hpp) | CurrentContextReadResult<TContextRecord>; private _status, _record and coupled constructor; public ForBound (lvalue only; mutable/const rvalues deleted), ForUnbound, ForUnsupportedContext, Status, IsBound, GetIfBound. Copies preserve a borrow, never ownership. |
| [CurrentContextBindingContract.hpp](../src/execution/CurrentContextBindingContract.hpp) | Internal Detail::CurrentContextBindingProviderTraits<TBindingProvider>; aliases Properties, ContextRecord, ReadResult, BindResult, ActualReadResult, RetireResult; validates composition, properties, provider lifetime, exact results and noexcept. |
| [Public umbrella](../src/ESPressio_Platform_CurrentContextBinding.hpp) | Narrow public entry. Also reachable from existing execution/main umbrellas. |

[Behavior, lifetimes and progressive use](../docs/CURRENT-CONTEXT-BINDING.MD).

## Maintained tooling

[CurrentContextBindingContractCases.cpp](../tests/execution/CurrentContextBindingContractCases.cpp): test-only Record, ShapeProvider, ContextRecord/ReadResult aliases, constructor/deleted members, BindCurrent/ReadCurrent/RetireCurrent, trait assertion and result-coupling main. EDP_BINDING_NEGATIVE_CASE invalidates one obligation per mode. This fixture is not a provider for consumers.

[Contract runner](../tests/run-current-context-binding-contract-tests.py) main accepts --compiler/--system, builds in temporary directories, requires expected rejection diagnostics and executes the positive fixture. CMake registers current_context_binding_contract_cases.

The [three-surface results demo](../demos/current-context-binding-results/README.MD) illustrates result coupling only. Concrete native execution belongs to its provider owner.
