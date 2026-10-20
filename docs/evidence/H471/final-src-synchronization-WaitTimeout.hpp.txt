#pragma once

#include <cstdint>

namespace ESPressio::Platform::Synchronization {

    /// Identifies how a synchronization wait should behave.
    enum class WaitMode : std::uint8_t {
        NoWait = 0,
        Finite = 1,
        Forever = 2
    };


    /// Platform-neutral relative wait request expressed in nanoseconds.
    class WaitTimeout final {

        private:

            // Wait specification.

            /// Selected wait behavior.
            WaitMode _mode;

            /// Relative wait duration in nanoseconds when the mode is Finite.
            std::uint64_t _nanoseconds;

        public:

            // Construction.

            /// Creates an immediate non-blocking wait request.
            static constexpr WaitTimeout NoWait() noexcept {
                return WaitTimeout(
                    WaitMode::NoWait,
                    0U
                );
            }

            /// Creates a finite relative wait request.
            static constexpr WaitTimeout ForNanoseconds(
                std::uint64_t nanoseconds
            ) noexcept {
                if (nanoseconds == 0U) { return NoWait(); }

                return WaitTimeout(
                    WaitMode::Finite,
                    nanoseconds
                );
            }

            /// Creates an indefinite wait request.
            static constexpr WaitTimeout Forever() noexcept {
                return WaitTimeout(
                    WaitMode::Forever,
                    0U
                );
            }


            // Wait inspection.

            /// Returns the selected wait mode.
            constexpr WaitMode Mode() const noexcept {
                return _mode;
            }

            /// Returns the finite relative duration in nanoseconds, or zero for non-finite modes.
            constexpr std::uint64_t Nanoseconds() const noexcept {
                return _nanoseconds;
            }

            /// Reports whether the request is non-blocking.
            constexpr bool IsNoWait() const noexcept {
                return _mode == WaitMode::NoWait;
            }

            /// Reports whether the request has a finite duration.
            constexpr bool IsFinite() const noexcept {
                return _mode == WaitMode::Finite;
            }

            /// Reports whether the request waits indefinitely.
            constexpr bool IsForever() const noexcept {
                return _mode == WaitMode::Forever;
            }

        private:

            // Internal construction.

            /// Creates a validated wait request from its normalized representation.
            constexpr WaitTimeout(
                WaitMode mode,
                std::uint64_t nanoseconds
            ) noexcept :
                _mode(mode),
                _nanoseconds(nanoseconds) {}

    };

} // ESPressio::Platform::Synchronization
