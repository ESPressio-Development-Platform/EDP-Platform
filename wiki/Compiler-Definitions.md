# Compiler Definitions and Conditional Compilation

No repository-defined compiler definitions were found in the audited PlatformIO/CMake/SDK configuration. Public behaviour is configured through C++ contracts/templates/providers rather than repository-specific preprocessor switches.

Platform/framework macros supplied externally are not EDP configuration knobs unless explicitly defined here.

> Audited against `cfcc637bb010579574a0df03d2c27071040ec53d` on `main` across 9 build-configuration file(s).

## Optional current-context binding

EDP_BINDING_NEGATIVE_CASE selects deliberately invalid test fixture shapes, not an application configuration predefinition. The new production vocabulary requires only the existing C++20 baseline and no SDK target macro.

See [exact reference](Reference-execution-CurrentContextBinding).
