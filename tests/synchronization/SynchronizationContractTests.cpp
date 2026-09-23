#include <cassert>
#include <cstdint>

#include <ESPressio_Platform.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;


    /// Minimal compile-time Mutex provider used to validate the Platform contract.
    class MutexProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::Mutex,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::MutexWaitResolutionNanoseconds,
                    1U
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            MutexProvider() = default;

            /// Prevents copying.
            MutexProvider(const MutexProvider&) = delete;

            /// Prevents copy assignment.
            MutexProvider& operator =(const MutexProvider&) = delete;

            /// Prevents moving.
            MutexProvider(MutexProvider&&) = delete;

            /// Prevents move assignment.
            MutexProvider& operator =(MutexProvider&&) = delete;

            /// Acquires the test mutex.
            ESPressio::Platform::Synchronization::LockAcquireResult Acquire(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::LockAcquireResult::Acquired;
            }

            /// Releases the test mutex.
            ESPressio::Platform::Synchronization::LockReleaseResult Release() noexcept {
                return ESPressio::Platform::Synchronization::LockReleaseResult::Released;
            }

    };


    /// Minimal compile-time RecursiveMutex provider used to validate the Platform contract.
    class RecursiveMutexProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::RecursiveMutex,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::RecursiveMutexWaitResolutionNanoseconds,
                    1U
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            RecursiveMutexProvider() = default;

            /// Prevents copying.
            RecursiveMutexProvider(const RecursiveMutexProvider&) = delete;

            /// Prevents copy assignment.
            RecursiveMutexProvider& operator =(const RecursiveMutexProvider&) = delete;

            /// Prevents moving.
            RecursiveMutexProvider(RecursiveMutexProvider&&) = delete;

            /// Prevents move assignment.
            RecursiveMutexProvider& operator =(RecursiveMutexProvider&&) = delete;

            /// Acquires the recursive test mutex.
            ESPressio::Platform::Synchronization::LockAcquireResult Acquire(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::LockAcquireResult::Acquired;
            }

            /// Releases the recursive test mutex.
            ESPressio::Platform::Synchronization::LockReleaseResult Release() noexcept {
                return ESPressio::Platform::Synchronization::LockReleaseResult::Released;
            }

    };


    /// Minimal compile-time ReadWriteMutex provider used to validate the Platform contract.
    class ReadWriteMutexProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::ReadWriteMutex,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::ReadWriteMutexWaitResolutionNanoseconds,
                    1U
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            ReadWriteMutexProvider() = default;

            /// Prevents copying.
            ReadWriteMutexProvider(const ReadWriteMutexProvider&) = delete;

            /// Prevents copy assignment.
            ReadWriteMutexProvider& operator =(const ReadWriteMutexProvider&) = delete;

            /// Prevents moving.
            ReadWriteMutexProvider(ReadWriteMutexProvider&&) = delete;

            /// Prevents move assignment.
            ReadWriteMutexProvider& operator =(ReadWriteMutexProvider&&) = delete;

            /// Acquires a shared read lock.
            ESPressio::Platform::Synchronization::LockAcquireResult AcquireRead(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::LockAcquireResult::Acquired;
            }

            /// Releases a shared read lock.
            ESPressio::Platform::Synchronization::LockReleaseResult ReleaseRead() noexcept {
                return ESPressio::Platform::Synchronization::LockReleaseResult::Released;
            }

            /// Acquires an exclusive write lock.
            ESPressio::Platform::Synchronization::LockAcquireResult AcquireWrite(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::LockAcquireResult::Acquired;
            }

            /// Releases an exclusive write lock.
            ESPressio::Platform::Synchronization::LockReleaseResult ReleaseWrite() noexcept {
                return ESPressio::Platform::Synchronization::LockReleaseResult::Released;
            }

    };


    /// Minimal compile-time CountingSemaphore provider used to validate the Platform contract.
    class CountingSemaphoreProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::CountingSemaphore,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::CountingSemaphoreWaitResolutionNanoseconds,
                    1U
                >,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::CountingSemaphoreSupportsInterruptRelease,
                    true
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            CountingSemaphoreProvider() = default;

            /// Prevents copying.
            CountingSemaphoreProvider(const CountingSemaphoreProvider&) = delete;

            /// Prevents copy assignment.
            CountingSemaphoreProvider& operator =(const CountingSemaphoreProvider&) = delete;

            /// Prevents moving.
            CountingSemaphoreProvider(CountingSemaphoreProvider&&) = delete;

            /// Prevents move assignment.
            CountingSemaphoreProvider& operator =(CountingSemaphoreProvider&&) = delete;

            /// Initializes the test semaphore.
            ESPressio::Platform::Synchronization::SemaphoreInitializationResult Initialize(
                std::uint32_t,
                std::uint32_t
            ) noexcept {
                return ESPressio::Platform::Synchronization::SemaphoreInitializationResult::Succeeded;
            }

            /// Acquires one test semaphore permit.
            ESPressio::Platform::Synchronization::SemaphoreAcquireResult Acquire(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::SemaphoreAcquireResult::Acquired;
            }

            /// Releases one test semaphore permit.
            ESPressio::Platform::Synchronization::SemaphoreReleaseResult Release() noexcept {
                return ESPressio::Platform::Synchronization::SemaphoreReleaseResult::Released;
            }

            /// Releases one test semaphore permit from interrupt context.
            ESPressio::Platform::Synchronization::SemaphoreReleaseResult ReleaseFromInterrupt() noexcept {
                return ESPressio::Platform::Synchronization::SemaphoreReleaseResult::Released;
            }

    };


    /// Minimal compile-time Signal provider used to validate the Platform contract.
    class SignalProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::Signal,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::SignalWaitResolutionNanoseconds,
                    1U
                >,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::SignalSupportsInterruptNotification,
                    true
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            SignalProvider() = default;

            /// Prevents copying.
            SignalProvider(const SignalProvider&) = delete;

            /// Prevents copy assignment.
            SignalProvider& operator =(const SignalProvider&) = delete;

            /// Prevents moving.
            SignalProvider(SignalProvider&&) = delete;

            /// Prevents move assignment.
            SignalProvider& operator =(SignalProvider&&) = delete;

            /// Latches the test signal.
            ESPressio::Platform::Synchronization::SignalNotifyResult Notify() noexcept {
                return ESPressio::Platform::Synchronization::SignalNotifyResult::Signaled;
            }

            /// Latches the test signal from interrupt context.
            ESPressio::Platform::Synchronization::SignalNotifyResult NotifyFromInterrupt() noexcept {
                return ESPressio::Platform::Synchronization::SignalNotifyResult::Signaled;
            }

            /// Waits for the test signal.
            ESPressio::Platform::Synchronization::SignalWaitResult Wait(
                ESPressio::Platform::Synchronization::WaitTimeout
            ) noexcept {
                return ESPressio::Platform::Synchronization::SignalWaitResult::Signaled;
            }

    };


    /// Minimal compile-time SpinLock provider used to validate the Platform contract.
    class SpinLockProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                ESPressio::Platform::Synchronization::SpinLock,
                Framework::PropertyValue<
                    ESPressio::Platform::Synchronization::SpinLockSupportsInterruptContext,
                    true
                >
            >
        >
    > {

        public:

            /// Creates the test provider.
            SpinLockProvider() = default;

            /// Prevents copying.
            SpinLockProvider(const SpinLockProvider&) = delete;

            /// Prevents copy assignment.
            SpinLockProvider& operator =(const SpinLockProvider&) = delete;

            /// Prevents moving.
            SpinLockProvider(SpinLockProvider&&) = delete;

            /// Prevents move assignment.
            SpinLockProvider& operator =(SpinLockProvider&&) = delete;

            /// Acquires the test spin lock.
            void Acquire() noexcept {}

            /// Acquires the test spin lock from interrupt context.
            ESPressio::Platform::Synchronization::SpinLockAcquireResult AcquireFromInterrupt() noexcept {
                return ESPressio::Platform::Synchronization::SpinLockAcquireResult::Acquired;
            }

            /// Releases the test spin lock.
            ESPressio::Platform::Synchronization::SpinLockReleaseResult Release() noexcept {
                return ESPressio::Platform::Synchronization::SpinLockReleaseResult::Released;
            }

            /// Releases the test spin lock from interrupt context.
            ESPressio::Platform::Synchronization::SpinLockReleaseResult ReleaseFromInterrupt() noexcept {
                return ESPressio::Platform::Synchronization::SpinLockReleaseResult::Released;
            }

    };


    /// Compile-time validation of the test Mutex provider.
    using MutexContract = ESPressio::Platform::Synchronization::Detail::MutexProviderTraits<MutexProvider>;

    /// Compile-time validation of the test RecursiveMutex provider.
    using RecursiveMutexContract = ESPressio::Platform::Synchronization::Detail::RecursiveMutexProviderTraits<RecursiveMutexProvider>;

    /// Compile-time validation of the test ReadWriteMutex provider.
    using ReadWriteMutexContract = ESPressio::Platform::Synchronization::Detail::ReadWriteMutexProviderTraits<ReadWriteMutexProvider>;

    /// Compile-time validation of the test CountingSemaphore provider.
    using CountingSemaphoreContract = ESPressio::Platform::Synchronization::Detail::CountingSemaphoreProviderTraits<CountingSemaphoreProvider>;

    /// Compile-time validation of the test Signal provider.
    using SignalContract = ESPressio::Platform::Synchronization::Detail::SignalProviderTraits<SignalProvider>;

    /// Compile-time validation of the test SpinLock provider.
    using SpinLockContract = ESPressio::Platform::Synchronization::Detail::SpinLockProviderTraits<SpinLockProvider>;

} // Test


/// Verifies wait representation and all synchronization capability contracts.
int main() {
    const auto immediate = ESPressio::Platform::Synchronization::WaitTimeout::NoWait();
    const auto finite = ESPressio::Platform::Synchronization::WaitTimeout::ForNanoseconds(
        250U
    );
    const auto forever = ESPressio::Platform::Synchronization::WaitTimeout::Forever();

    assert(
        immediate.IsNoWait()
    );

    assert(
        finite.IsFinite()
    );

    assert(
        finite.Nanoseconds() == 250U
    );

    assert(
        forever.IsForever()
    );

    static_assert(
        Test::CountingSemaphoreContract::Properties::template Value<
            ESPressio::Platform::Synchronization::CountingSemaphoreSupportsInterruptRelease
        >,
        "CountingSemaphore test provider must advertise interrupt release support"
    );

    static_assert(
        Test::SignalContract::Properties::template Value<
            ESPressio::Platform::Synchronization::SignalSupportsInterruptNotification
        >,
        "Signal test provider must advertise interrupt notification support"
    );

    static_assert(
        Test::SpinLockContract::Properties::template Value<
            ESPressio::Platform::Synchronization::SpinLockSupportsInterruptContext
        >,
        "SpinLock test provider must advertise interrupt-context support"
    );

    return 0;
}
