# Build, Test and Source Map

C++20 is required. `src/` contains contracts only; concrete implementations belong in Arduino, ESP-IDF, FreeRTOS or Portable provider repositories according to the API that actually owns the implementation. Tests protect contract shape, type properties, timeout/result vocabulary and snapshot semantics.

## Optional current-context binding

CMake registers current_context_binding_contract_cases: one valid result/provider shape and ten independently invalid metadata, type, noexcept or lifetime shapes. GCC/Clang Debug/Release all pass alongside the previous six CTest entries. The new three-surface result demo illustrates payload coupling only; native owner tests establish runtime properties.

See [exact reference](Reference-execution-CurrentContextBinding).
