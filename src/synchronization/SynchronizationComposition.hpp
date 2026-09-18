#pragma once

#include <cstdint>

#include "../platform/PlatformComposition.hpp"

namespace ESPressio::Platform::Synchronization {

    /// Exclusive Platform capability supplying a non-recursive mutex implementation.
    struct Mutex final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Exclusive Platform capability supplying a recursive mutex implementation.
    struct RecursiveMutex final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Exclusive Platform capability supplying a shared-read/exclusive-write mutex implementation.
    struct ReadWriteMutex final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Exclusive Platform capability supplying a counting semaphore implementation.
    struct CountingSemaphore final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Exclusive Platform capability supplying a latched wake signal implementation.
    struct Signal final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Exclusive Platform capability supplying a short-duration spin lock implementation.
    struct SpinLock final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Native wait resolution, in nanoseconds, advertised by a Mutex provider.
    struct MutexWaitResolutionNanoseconds final : Framework::Property<Mutex, std::uint64_t> {};


    /// Native wait resolution, in nanoseconds, advertised by a RecursiveMutex provider.
    struct RecursiveMutexWaitResolutionNanoseconds final : Framework::Property<RecursiveMutex, std::uint64_t> {};


    /// Native wait resolution, in nanoseconds, advertised by a ReadWriteMutex provider.
    struct ReadWriteMutexWaitResolutionNanoseconds final : Framework::Property<ReadWriteMutex, std::uint64_t> {};


    /// Native wait resolution, in nanoseconds, advertised by a CountingSemaphore provider.
    struct CountingSemaphoreWaitResolutionNanoseconds final : Framework::Property<CountingSemaphore, std::uint64_t> {};


    /// Indicates whether a CountingSemaphore provider supports interrupt-context release.
    struct CountingSemaphoreSupportsInterruptRelease final : Framework::Property<CountingSemaphore, bool> {};


    /// Native wait resolution, in nanoseconds, advertised by a Signal provider.
    struct SignalWaitResolutionNanoseconds final : Framework::Property<Signal, std::uint64_t> {};


    /// Indicates whether a Signal provider supports interrupt-context notification.
    struct SignalSupportsInterruptNotification final : Framework::Property<Signal, bool> {};


    /// Indicates whether a SpinLock provider supports interrupt-context acquisition and release.
    struct SpinLockSupportsInterruptContext final : Framework::Property<SpinLock, bool> {};

} // ESPressio::Platform::Synchronization
