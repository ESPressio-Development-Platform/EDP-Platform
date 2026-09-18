#include <Arduino.h>

#include <ESPressio_Platform.hpp>
#include <ESPressio_Platform_FreeRTOS.hpp>

namespace Demo {

    /// Runs basic mutex and latched-signal operations through FreeRTOS providers.
    int Run() noexcept {
        using MutexProvider = ESPressio::Platform::FreeRTOS::Synchronization::MutexProvider;
        using SignalProvider = ESPressio::Platform::FreeRTOS::Synchronization::SignalProvider;

        MutexProvider mutex;
        SignalProvider signal;

        if (
            mutex.Acquire(
                ESPressio::Platform::Synchronization::WaitTimeout::NoWait()
            ) != ESPressio::Platform::Synchronization::LockAcquireResult::Acquired
        ) {
            return 1;
        }

        if (
            mutex.Release() !=
            ESPressio::Platform::Synchronization::LockReleaseResult::Released
        ) {
            return 2;
        }

        if (
            signal.Notify() !=
            ESPressio::Platform::Synchronization::SignalNotifyResult::Signaled
        ) {
            return 3;
        }

        return signal.Wait(
            ESPressio::Platform::Synchronization::WaitTimeout::NoWait()
        ) == ESPressio::Platform::Synchronization::SignalWaitResult::Signaled
            ? 0
            : 4;
    }

} // Demo

/// Runs the synchronization demonstration once.
void setup() {
    static_cast<void>(
        Demo::Run()
    );
}

/// Leaves the demonstration idle after the one-time run.
void loop() {}
