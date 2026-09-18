#pragma once

#include <cstdint>

namespace ESPressio::Platform::Synchronization {

    /// Outcome from acquiring an exclusive or shared lock.
    enum class LockAcquireResult : std::uint8_t {
        Acquired = 0,
        TimedOut = 1,
        ProviderFailure = 2
    };


    /// Outcome from releasing an exclusive or shared lock.
    enum class LockReleaseResult : std::uint8_t {
        Released = 0,
        InvalidOwnership = 1,
        ProviderFailure = 2
    };


    /// Outcome from initializing a counting semaphore.
    enum class SemaphoreInitializationResult : std::uint8_t {
        Succeeded = 0,
        InvalidMaximumCount = 1,
        InvalidInitialCount = 2,
        AlreadyInitialized = 3,
        ProviderFailure = 4
    };


    /// Outcome from acquiring one counting-semaphore permit.
    enum class SemaphoreAcquireResult : std::uint8_t {
        Acquired = 0,
        TimedOut = 1,
        NotInitialized = 2,
        ProviderFailure = 3
    };


    /// Outcome from releasing one counting-semaphore permit.
    enum class SemaphoreReleaseResult : std::uint8_t {
        Released = 0,
        CapacityReached = 1,
        NotInitialized = 2,
        UnsupportedInterruptContext = 3,
        ProviderFailure = 4
    };


    /// Outcome from notifying a latched signal.
    enum class SignalNotifyResult : std::uint8_t {
        Signaled = 0,
        UnsupportedInterruptContext = 1,
        ProviderFailure = 2
    };


    /// Outcome from waiting for a latched signal.
    enum class SignalWaitResult : std::uint8_t {
        Signaled = 0,
        TimedOut = 1,
        ProviderFailure = 2
    };


    /// Outcome from acquiring a spin lock in interrupt context.
    enum class SpinLockAcquireResult : std::uint8_t {
        Acquired = 0,
        UnsupportedInterruptContext = 1
    };


    /// Outcome from releasing a spin lock.
    enum class SpinLockReleaseResult : std::uint8_t {
        Released = 0,
        UnsupportedInterruptContext = 1
    };

} // ESPressio::Platform::Synchronization
