#include <array>
#include <cassert>
#include <cstddef>

#include <ESPressio_Platform.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;


    /// Minimal caller-storage-backed execution provider used for contract validation.
    class ExecutionProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Execution::ExecutionContext,
                Framework::PropertyValue<ESPressio::Platform::Execution::CallerSuppliedStorage, true>,
                Framework::PropertyValue<ESPressio::Platform::Execution::SupportsPriority, true>,
                Framework::PropertyValue<ESPressio::Platform::Execution::SupportsProcessorAffinity, false>,
                Framework::PropertyValue<ESPressio::Platform::Execution::SupportsStackTelemetry, false>,
                Framework::PropertyValue<ESPressio::Platform::Execution::ControlStorageBytes, 16U>,
                Framework::PropertyValue<ESPressio::Platform::Execution::ControlStorageAlignment, alignof(std::max_align_t)>,
                Framework::PropertyValue<ESPressio::Platform::Execution::StackStorageAlignment, alignof(std::max_align_t)>,
                Framework::PropertyValue<ESPressio::Platform::Execution::StackAllocationGranularityBytes, 1U>,
                Framework::PropertyValue<ESPressio::Platform::Execution::JoinWaitResolutionNanoseconds, 1U>
            >
        >
    > {

        private:

            // Execution state.

            /// Entry function bound at initialization.
            ESPressio::Platform::Execution::ExecutionEntry _entry = nullptr;

            /// Entry parameter bound at initialization.
            void* _parameter = nullptr;

            /// Indicates whether initialization completed.
            bool _initialized = false;

            /// Indicates whether execution completed.
            bool _completed = false;

        public:

            // Construction and lifetime.

            /// Creates an uninitialized test provider.
            ExecutionProvider() = default;

            /// Prevents copying.
            ExecutionProvider(const ExecutionProvider&) = delete;

            /// Prevents copy assignment.
            ExecutionProvider& operator =(const ExecutionProvider&) = delete;

            /// Prevents moving.
            ExecutionProvider(ExecutionProvider&&) = delete;

            /// Prevents move assignment.
            ExecutionProvider& operator =(ExecutionProvider&&) = delete;


            // Lifecycle.

            /// Binds caller storage and one execution entry.
            ESPressio::Platform::Execution::ExecutionInitializationResult Initialize(
                const ESPressio::Platform::Execution::ExecutionStorage& storage,
                const ESPressio::Platform::Execution::ExecutionConfiguration&,
                ESPressio::Platform::Execution::ExecutionEntry entry,
                void* parameter
            ) noexcept {
                if (
                    storage.ControlAddress == nullptr ||
                    storage.StackAddress == nullptr ||
                    entry == nullptr
                ) {
                    return ESPressio::Platform::Execution::ExecutionInitializationResult::InvalidStorage;
                }

                _entry = entry;
                _parameter = parameter;
                _initialized = true;
                _completed = false;

                return ESPressio::Platform::Execution::ExecutionInitializationResult::Succeeded;
            }

            /// Executes the bound entry synchronously for the test.
            ESPressio::Platform::Execution::ExecutionStartResult Start() noexcept {
                if (!_initialized || _entry == nullptr) {
                    return ESPressio::Platform::Execution::ExecutionStartResult::InvalidState;
                }

                _entry(
                    _parameter
                );
                _completed = true;

                return ESPressio::Platform::Execution::ExecutionStartResult::Succeeded;
            }

            /// Reports completion without blocking.
            ESPressio::Platform::Execution::ExecutionJoinResult Join(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return _completed
                    ? ESPressio::Platform::Execution::ExecutionJoinResult::Succeeded
                    : ESPressio::Platform::Execution::ExecutionJoinResult::TimedOut;
            }

            /// Retires the test execution context.
            ESPressio::Platform::Execution::ExecutionDestroyResult Destroy() noexcept {
                if (!_initialized || !_completed) {
                    return ESPressio::Platform::Execution::ExecutionDestroyResult::InvalidState;
                }

                _initialized = false;
                _completed = false;
                _entry = nullptr;
                _parameter = nullptr;

                return ESPressio::Platform::Execution::ExecutionDestroyResult::Succeeded;
            }


            // Inspection.

            /// Reports that the synchronous test is not a distinct native context.
            bool IsCurrentContext() const noexcept {
                return false;
            }

            /// Reports unavailable stack telemetry.
            ESPressio::Platform::Execution::ExecutionStackTelemetry GetStackTelemetry() const noexcept {
                return {};
            }

            /// Yields the synchronous test execution runtime.
            static void Yield() noexcept {}

    };


    /// Compile-time validation of the fake execution provider.
    using ExecutionContract = ESPressio::Platform::Execution::Detail::ExecutionContextProviderTraits<ExecutionProvider>;


    /// Marks the execution body as having run.
    void Entry(
        void* parameter
    ) noexcept {
        *static_cast<bool*>(parameter) = true;
    }

} // Test


/// Verifies the execution contract against one deterministic test provider.
int main() {
    alignas(std::max_align_t) std::array<std::byte, 16U> control{};
    alignas(std::max_align_t) std::array<std::byte, 128U> stack{};
    bool executed = false;

    Test::ExecutionProvider provider;

    const ESPressio::Platform::Execution::ExecutionStorage storage {
        control.data(),
        control.size(),
        stack.data(),
        stack.size()
    };

    ESPressio::Platform::Execution::ExecutionConfiguration configuration;

    assert(
        configuration.Priority ==
        ESPressio::Platform::Execution::ExecutionPriority::Normal
    );

    configuration.Priority = ESPressio::Platform::Execution::ExecutionPriority::Critical;

    assert(
        provider.Initialize(
            storage,
            configuration,
            &Test::Entry,
            &executed
        ) == ESPressio::Platform::Execution::ExecutionInitializationResult::Succeeded
    );

    assert(
        provider.Start() ==
        ESPressio::Platform::Execution::ExecutionStartResult::Succeeded
    );

    assert(
        provider.Join(
            ESPressio::Platform::Synchronization::WaitTimeout::NoWait()
        ) == ESPressio::Platform::Execution::ExecutionJoinResult::Succeeded
    );

    assert(
        executed
    );

    assert(
        provider.Destroy() ==
        ESPressio::Platform::Execution::ExecutionDestroyResult::Succeeded
    );

    return 0;
}
