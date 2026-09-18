#pragma once

#include <type_traits>
#include <utility>

#include "SynchronizationComposition.hpp"
#include "SynchronizationTypes.hpp"
#include "WaitTimeout.hpp"

namespace ESPressio::Platform::Synchronization::Detail {

    /// Validates the common object properties required from a synchronization provider.
    template<class TSynchronizationProvider>
    struct SynchronizationObjectTraits {

        static_assert(
            std::is_default_constructible_v<TSynchronizationProvider>,
            "Synchronization providers must be default constructible"
        );

        static_assert(
            !std::is_copy_constructible_v<TSynchronizationProvider> &&
            !std::is_copy_assignable_v<TSynchronizationProvider>,
            "Synchronization providers must not be copyable"
        );

        static_assert(
            !std::is_move_constructible_v<TSynchronizationProvider> &&
            !std::is_move_assignable_v<TSynchronizationProvider>,
            "Synchronization providers must not be movable"
        );

    };


    /// Validates the complete contract required from a Mutex capability provider.
    template<class TMutexProvider>
    struct MutexProviderTraits : SynchronizationObjectTraits<TMutexProvider> {

        static_assert(
            TMutexProvider::CompositionCapabilities::template Contains<Mutex>,
            "Mutex provider must supply the Mutex capability"
        );

        /// Properties advertised for the Mutex capability.
        using Properties = typename TMutexProvider::CompositionCapabilities::template PropertiesFor<Mutex>;

        static_assert(
            Properties::template Contains<MutexWaitResolutionNanoseconds>,
            "Mutex provider must advertise MutexWaitResolutionNanoseconds"
        );

        static_assert(
            Properties::template Value<MutexWaitResolutionNanoseconds> > 0U,
            "Mutex provider wait resolution must be greater than zero"
        );

        /// Return type produced by mutex acquisition.
        using AcquireResult = decltype(
            std::declval<TMutexProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );

        /// Return type produced by mutex release.
        using ReleaseResult = decltype(
            std::declval<TMutexProvider&>().Release()
        );

        static_assert(
            std::is_same_v<AcquireResult, LockAcquireResult>,
            "Mutex Acquire must return LockAcquireResult"
        );

        static_assert(
            std::is_same_v<ReleaseResult, LockReleaseResult>,
            "Mutex Release must return LockReleaseResult"
        );

    };


    /// Validates the complete contract required from a RecursiveMutex capability provider.
    template<class TRecursiveMutexProvider>
    struct RecursiveMutexProviderTraits : SynchronizationObjectTraits<TRecursiveMutexProvider> {

        static_assert(
            TRecursiveMutexProvider::CompositionCapabilities::template Contains<RecursiveMutex>,
            "RecursiveMutex provider must supply the RecursiveMutex capability"
        );

        /// Properties advertised for the RecursiveMutex capability.
        using Properties = typename TRecursiveMutexProvider::CompositionCapabilities::template PropertiesFor<RecursiveMutex>;

        static_assert(
            Properties::template Contains<RecursiveMutexWaitResolutionNanoseconds>,
            "RecursiveMutex provider must advertise RecursiveMutexWaitResolutionNanoseconds"
        );

        /// Return type produced by recursive-mutex acquisition.
        using AcquireResult = decltype(
            std::declval<TRecursiveMutexProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );

        /// Return type produced by recursive-mutex release.
        using ReleaseResult = decltype(
            std::declval<TRecursiveMutexProvider&>().Release()
        );

        static_assert(
            std::is_same_v<AcquireResult, LockAcquireResult>,
            "RecursiveMutex Acquire must return LockAcquireResult"
        );

        static_assert(
            std::is_same_v<ReleaseResult, LockReleaseResult>,
            "RecursiveMutex Release must return LockReleaseResult"
        );

    };


    /// Validates the complete contract required from a ReadWriteMutex capability provider.
    template<class TReadWriteMutexProvider>
    struct ReadWriteMutexProviderTraits : SynchronizationObjectTraits<TReadWriteMutexProvider> {

        static_assert(
            TReadWriteMutexProvider::CompositionCapabilities::template Contains<ReadWriteMutex>,
            "ReadWriteMutex provider must supply the ReadWriteMutex capability"
        );

        /// Properties advertised for the ReadWriteMutex capability.
        using Properties = typename TReadWriteMutexProvider::CompositionCapabilities::template PropertiesFor<ReadWriteMutex>;

        static_assert(
            Properties::template Contains<ReadWriteMutexWaitResolutionNanoseconds>,
            "ReadWriteMutex provider must advertise ReadWriteMutexWaitResolutionNanoseconds"
        );

        /// Return type produced by shared-read acquisition.
        using ReadAcquireResult = decltype(
            std::declval<TReadWriteMutexProvider&>().AcquireRead(
                std::declval<WaitTimeout>()
            )
        );

        /// Return type produced by shared-read release.
        using ReadReleaseResult = decltype(
            std::declval<TReadWriteMutexProvider&>().ReleaseRead()
        );

        /// Return type produced by exclusive-write acquisition.
        using WriteAcquireResult = decltype(
            std::declval<TReadWriteMutexProvider&>().AcquireWrite(
                std::declval<WaitTimeout>()
            )
        );

        /// Return type produced by exclusive-write release.
        using WriteReleaseResult = decltype(
            std::declval<TReadWriteMutexProvider&>().ReleaseWrite()
        );

        static_assert(
            std::is_same_v<ReadAcquireResult, LockAcquireResult> &&
            std::is_same_v<WriteAcquireResult, LockAcquireResult>,
            "ReadWriteMutex acquisition methods must return LockAcquireResult"
        );

        static_assert(
            std::is_same_v<ReadReleaseResult, LockReleaseResult> &&
            std::is_same_v<WriteReleaseResult, LockReleaseResult>,
            "ReadWriteMutex release methods must return LockReleaseResult"
        );

    };


    /// Validates the complete contract required from a CountingSemaphore capability provider.
    template<class TCountingSemaphoreProvider>
    struct CountingSemaphoreProviderTraits : SynchronizationObjectTraits<TCountingSemaphoreProvider> {

        static_assert(
            TCountingSemaphoreProvider::CompositionCapabilities::template Contains<CountingSemaphore>,
            "CountingSemaphore provider must supply the CountingSemaphore capability"
        );

        /// Properties advertised for the CountingSemaphore capability.
        using Properties = typename TCountingSemaphoreProvider::CompositionCapabilities::template PropertiesFor<CountingSemaphore>;

        static_assert(
            Properties::template Contains<CountingSemaphoreWaitResolutionNanoseconds>,
            "CountingSemaphore provider must advertise CountingSemaphoreWaitResolutionNanoseconds"
        );

        static_assert(
            Properties::template Contains<CountingSemaphoreSupportsInterruptRelease>,
            "CountingSemaphore provider must advertise CountingSemaphoreSupportsInterruptRelease"
        );

        /// Return type produced by semaphore initialization.
        using InitializeResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Initialize(
                std::declval<std::uint32_t>(),
                std::declval<std::uint32_t>()
            )
        );

        /// Return type produced by permit acquisition.
        using AcquireResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Acquire(
                std::declval<WaitTimeout>()
            )
        );

        /// Return type produced by permit release.
        using ReleaseResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().Release()
        );

        /// Return type produced by interrupt-context permit release.
        using InterruptReleaseResult = decltype(
            std::declval<TCountingSemaphoreProvider&>().ReleaseFromInterrupt()
        );

        static_assert(
            std::is_same_v<InitializeResult, SemaphoreInitializationResult>,
            "CountingSemaphore Initialize must return SemaphoreInitializationResult"
        );

        static_assert(
            std::is_same_v<AcquireResult, SemaphoreAcquireResult>,
            "CountingSemaphore Acquire must return SemaphoreAcquireResult"
        );

        static_assert(
            std::is_same_v<ReleaseResult, SemaphoreReleaseResult> &&
            std::is_same_v<InterruptReleaseResult, SemaphoreReleaseResult>,
            "CountingSemaphore release methods must return SemaphoreReleaseResult"
        );

    };


    /// Validates the complete contract required from a Signal capability provider.
    template<class TSignalProvider>
    struct SignalProviderTraits : SynchronizationObjectTraits<TSignalProvider> {

        static_assert(
            TSignalProvider::CompositionCapabilities::template Contains<Signal>,
            "Signal provider must supply the Signal capability"
        );

        /// Properties advertised for the Signal capability.
        using Properties = typename TSignalProvider::CompositionCapabilities::template PropertiesFor<Signal>;

        static_assert(
            Properties::template Contains<SignalWaitResolutionNanoseconds>,
            "Signal provider must advertise SignalWaitResolutionNanoseconds"
        );

        static_assert(
            Properties::template Contains<SignalSupportsInterruptNotification>,
            "Signal provider must advertise SignalSupportsInterruptNotification"
        );

        /// Return type produced by ordinary signal notification.
        using NotifyResult = decltype(
            std::declval<TSignalProvider&>().Notify()
        );

        /// Return type produced by interrupt-context signal notification.
        using InterruptNotifyResult = decltype(
            std::declval<TSignalProvider&>().NotifyFromInterrupt()
        );

        /// Return type produced by waiting for a signal.
        using WaitResult = decltype(
            std::declval<TSignalProvider&>().Wait(
                std::declval<WaitTimeout>()
            )
        );

        static_assert(
            std::is_same_v<NotifyResult, SignalNotifyResult> &&
            std::is_same_v<InterruptNotifyResult, SignalNotifyResult>,
            "Signal notify methods must return SignalNotifyResult"
        );

        static_assert(
            std::is_same_v<WaitResult, SignalWaitResult>,
            "Signal Wait must return SignalWaitResult"
        );

    };


    /// Validates the complete contract required from a SpinLock capability provider.
    template<class TSpinLockProvider>
    struct SpinLockProviderTraits : SynchronizationObjectTraits<TSpinLockProvider> {

        static_assert(
            TSpinLockProvider::CompositionCapabilities::template Contains<SpinLock>,
            "SpinLock provider must supply the SpinLock capability"
        );

        /// Properties advertised for the SpinLock capability.
        using Properties = typename TSpinLockProvider::CompositionCapabilities::template PropertiesFor<SpinLock>;

        static_assert(
            Properties::template Contains<SpinLockSupportsInterruptContext>,
            "SpinLock provider must advertise SpinLockSupportsInterruptContext"
        );

        /// Return type produced by ordinary-context blocking acquisition.
        using AcquireResult = decltype(
            std::declval<TSpinLockProvider&>().Acquire()
        );

        /// Return type produced by interrupt-context acquisition.
        using InterruptAcquireResult = decltype(
            std::declval<TSpinLockProvider&>().AcquireFromInterrupt()
        );

        /// Return type produced by ordinary-context release.
        using ReleaseResult = decltype(
            std::declval<TSpinLockProvider&>().Release()
        );

        /// Return type produced by interrupt-context release.
        using InterruptReleaseResult = decltype(
            std::declval<TSpinLockProvider&>().ReleaseFromInterrupt()
        );

        static_assert(
            std::is_same_v<AcquireResult, void>,
            "SpinLock Acquire must return void"
        );

        static_assert(
            std::is_same_v<InterruptAcquireResult, SpinLockAcquireResult>,
            "SpinLock AcquireFromInterrupt must return SpinLockAcquireResult"
        );

        static_assert(
            std::is_same_v<ReleaseResult, SpinLockReleaseResult> &&
            std::is_same_v<InterruptReleaseResult, SpinLockReleaseResult>,
            "SpinLock release methods must return SpinLockReleaseResult"
        );

    };

} // ESPressio::Platform::Synchronization::Detail
