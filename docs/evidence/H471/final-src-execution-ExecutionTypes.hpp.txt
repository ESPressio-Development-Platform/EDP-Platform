#pragma once

#include <cstddef>
#include <cstdint>

namespace ESPressio::Platform::Execution {

    /// Portable scheduler-preference levels for one execution context.
    enum class ExecutionPriority : std::uint8_t {
        Low = 0,
        Normal = 1,
        High = 2,
        Critical = 3
    };


    /// Selects whether an execution context may run on any processor or one requested processor.
    enum class ProcessorAffinityMode : std::uint8_t {
        Any = 0,
        Specific = 1
    };


    /// Outcome from initializing one execution context.
    enum class ExecutionInitializationResult : std::uint8_t {
        Succeeded = 0,
        AlreadyInitialized = 1,
        InvalidStorage = 2,
        InvalidConfiguration = 3,
        UnsupportedAffinity = 4,
        ProviderFailure = 5
    };


    /// Outcome from starting one initialized execution context.
    enum class ExecutionStartResult : std::uint8_t {
        Succeeded = 0,
        InvalidState = 1,
        ProviderFailure = 2
    };


    /// Outcome from joining one execution context.
    enum class ExecutionJoinResult : std::uint8_t {
        Succeeded = 0,
        TimedOut = 1,
        SelfJoin = 2,
        InvalidState = 3,
        ProviderFailure = 4
    };


    /// Outcome from destroying one execution context.
    enum class ExecutionDestroyResult : std::uint8_t {
        Succeeded = 0,
        InvalidState = 1,
        ProviderFailure = 2
    };


    /// Processor-affinity request using ESPressio vocabulary.
    class ProcessorAffinity final {

        private:

            // Affinity specification.

            /// Selected affinity mode.
            ProcessorAffinityMode _mode;

            /// Zero-based processor index when the mode is Specific.
            std::uint32_t _processorIndex;


            // Internal construction.

            /// Creates a normalized processor-affinity request.
            constexpr ProcessorAffinity(
                ProcessorAffinityMode mode,
                std::uint32_t processorIndex
            ) noexcept :
                _mode(mode),
                _processorIndex(processorIndex) {}

        public:

            // Construction.

            /// Requests that the provider select any suitable processor.
            static constexpr ProcessorAffinity Any() noexcept {
                return ProcessorAffinity(
                    ProcessorAffinityMode::Any,
                    0U
                );
            }

            /// Requests one specific zero-based processor index.
            static constexpr ProcessorAffinity Specific(
                std::uint32_t processorIndex
            ) noexcept {
                return ProcessorAffinity(
                    ProcessorAffinityMode::Specific,
                    processorIndex
                );
            }


            // Affinity inspection.

            /// Returns the selected affinity mode.
            constexpr ProcessorAffinityMode Mode() const noexcept {
                return _mode;
            }

            /// Returns the requested processor index.
            constexpr std::uint32_t ProcessorIndex() const noexcept {
                return _processorIndex;
            }

            /// Reports whether any processor is acceptable.
            constexpr bool IsAny() const noexcept {
                return _mode == ProcessorAffinityMode::Any;
            }

    };


    /// Caller-owned storage supplied to one native execution context.
    struct ExecutionStorage final {

        // Native control storage.

        /// Address of caller-owned native control storage.
        void* ControlAddress = nullptr;

        /// Number of bytes available at ControlAddress.
        std::size_t ControlBytes = 0U;


        // Native stack storage.

        /// Address of caller-owned execution-stack storage.
        void* StackAddress = nullptr;

        /// Number of bytes available at StackAddress.
        std::size_t StackBytes = 0U;

    };


    /// Platform-neutral creation settings for one execution context.
    struct ExecutionConfiguration final {

        // Scheduling configuration.

        /// Portable scheduler-preference level mapped by the concrete provider.
        ExecutionPriority Priority = ExecutionPriority::Normal;

        /// Requested processor affinity.
        ProcessorAffinity Affinity = ProcessorAffinity::Any();


        // Diagnostic configuration.

        /// Optional diagnostic name; providers may copy or ignore it.
        const char* Name = nullptr;

    };


    /// Portable stack-use evidence returned by an execution provider.
    struct ExecutionStackTelemetry final {

        // Telemetry result.

        /// Indicates whether this provider can report the value for this context.
        bool Available = false;

        /// Minimum number of stack bytes that remained unused during the context lifetime.
        std::uint32_t MinimumFreeBytes = 0U;

    };


    /// Native execution entry function boundary.
    using ExecutionEntry = void (*)(
        void*
    ) noexcept;

} // ESPressio::Platform::Execution
