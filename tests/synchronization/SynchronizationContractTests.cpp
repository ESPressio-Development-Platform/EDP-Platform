#include <cassert>
#include <cstdint>

#include <ESPressio_Platform.hpp>

namespace Test {

    namespace Framework = ESPressio::System::CompositionFramework;


    /// Minimal compile-time Mutex provider used to validate the Platform contract.
    class MutexProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Provides<
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


    /// Minimal compile-time Signal provider used to validate ISR capability metadata.
    class SignalProvider final : public Framework::Provider<
        ESPressio::Platform::Domain,
        Framework::Provides<
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


    /// Compile-time validation of the test Mutex provider.
    using MutexContract = ESPressio::Platform::Synchronization::Detail::MutexProviderTraits<MutexProvider>;

    /// Compile-time validation of the test Signal provider.
    using SignalContract = ESPressio::Platform::Synchronization::Detail::SignalProviderTraits<SignalProvider>;

} // Test


/// Verifies portable wait representation and selected contract metadata.
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
        Test::SignalContract::Properties::template Value<
            ESPressio::Platform::Synchronization::SignalSupportsInterruptNotification
        >,
        "Signal test provider must advertise interrupt notification support"
    );

    return 0;
}
