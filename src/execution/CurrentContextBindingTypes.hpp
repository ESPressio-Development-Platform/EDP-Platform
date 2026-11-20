#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>

namespace ESPressio::Platform::Execution {

    /// Outcome of binding one borrowed record to the current ordinary execution context.
    enum class CurrentContextBindResult : std::uint8_t {
        /// Record accepted into an empty binding.
        Bound = 0U,
        /// An occupied binding was preserved.
        AlreadyBound = 1U,
        /// Operation rejected before accessing task-local binding storage.
        UnsupportedContext = 2U
    };


    /// Availability of the current ordinary context's borrowed record.
    enum class CurrentContextReadStatus : std::uint8_t {
        /// A borrowed record is available.
        Bound = 0U,
        /// An ordinary context has no borrowed record.
        Unbound = 1U,
        /// The caller is outside supported ordinary task execution.
        UnsupportedContext = 2U
    };


    /// Outcome of retiring only the expected current-context binding.
    enum class CurrentContextRetireResult : std::uint8_t {
        /// The matching borrowed record was cleared.
        Retired = 0U,
        /// The ordinary context already had no record.
        NotBound = 1U,
        /// Another record was preserved.
        DifferentBinding = 2U,
        /// Retirement rejected without touching task-local binding storage.
        UnsupportedContext = 3U
    };


    /// Couples binding availability to its borrowed payload; unavailable results cannot carry a record.
    /// The owner keeps a bound record address stable through retirement and every outstanding borrow.
    /// @tparam TContextRecord Unqualified object Type owned by application composition or its lifecycle owner.
    template<class TContextRecord>
    class CurrentContextReadResult final {

        static_assert(
            std::is_object_v<TContextRecord> &&
            std::is_same_v<TContextRecord, std::remove_cv_t<TContextRecord>>,
            "Current-context records must be unqualified object Types"
        );

        private:

            // Coupled availability and borrowed storage.

            /// Availability supplied only by one of the named factories.
            CurrentContextReadStatus _status;

            /// Non-null only for a Bound result; ownership never transfers.
            const TContextRecord* _record;

            /// Constructs only the status/payload combinations exposed by named factories.
            constexpr CurrentContextReadResult(
                CurrentContextReadStatus status,
                const TContextRecord* record
            ) noexcept
                : _status(status),
                  _record(record) {
            }

        public:

            // Valid result construction.

            /// Borrows an existing owner record without copying it or extending its lifetime.
            static constexpr CurrentContextReadResult ForBound(
                const TContextRecord& record
            ) noexcept {
                return CurrentContextReadResult(
                    CurrentContextReadStatus::Bound,
                    std::addressof(record)
                );
            }

            /// Prevents the obvious temporary-record lifetime error.
            static CurrentContextReadResult ForBound(TContextRecord&&) = delete;

            /// Prevents borrowing a const temporary record.
            static CurrentContextReadResult ForBound(const TContextRecord&&) = delete;

            /// Reports an ordinary context that has not been bound.
            static constexpr CurrentContextReadResult ForUnbound() noexcept {
                return CurrentContextReadResult(
                    CurrentContextReadStatus::Unbound,
                    nullptr
                );
            }

            /// Reports a context, including an ISR, that this provider cannot bind.
            static constexpr CurrentContextReadResult ForUnsupportedContext() noexcept {
                return CurrentContextReadResult(
                    CurrentContextReadStatus::UnsupportedContext,
                    nullptr
                );
            }


            // Availability and checked payload inspection.

            /// Returns the precise availability classification.
            constexpr CurrentContextReadStatus Status() const noexcept {
                return _status;
            }

            /// Indicates whether this result contains a bound borrowed record.
            constexpr bool IsBound() const noexcept {
                return _status == CurrentContextReadStatus::Bound;
            }

            /// Returns the borrowed record only when Bound, otherwise nullptr.
            constexpr const TContextRecord* GetIfBound() const noexcept {
                return _record;
            }

    };

} // ESPressio::Platform::Execution
