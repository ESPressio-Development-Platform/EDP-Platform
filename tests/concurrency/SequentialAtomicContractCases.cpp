#include <atomic>
#include <cstdint>
#include <ESPressio_Platform.hpp>

#ifndef EDP_PLATFORM_SC_NEGATIVE_CASE
#define EDP_PLATFORM_SC_NEGATIVE_CASE 0
#endif

namespace ESPressio::Platform::Concurrency::Tests {

    /// Host-only shape fixture; each negative mode violates one optional provider obligation.
    class ShapeWord final {

        private:

            // Fixed test storage.

            /// Actual host atomic keeps the positive fixture's ordering promise valid.
            std::atomic<std::uint32_t> _value{0U};

        public:

            // Word lifetime.

            /// Initializes one zero word before any concurrent access.
            ShapeWord() noexcept = default;

            /// Prevents copied atomic storage identity.
            ShapeWord(const ShapeWord&) = delete;

            /// Prevents reassignment of atomic storage identity.
            ShapeWord& operator =(const ShapeWord&) = delete;

            /// Prevents moved atomic storage identity.
            ShapeWord(ShapeWord&&) = delete;

            /// Prevents move assignment of atomic storage identity.
            ShapeWord& operator =(ShapeWord&&) = delete;


            // Base operations.

            /// Reads a relaxed test value.
            std::uint32_t LoadRelaxed() const noexcept {
                return _value.load(std::memory_order_relaxed);
            }

            /// Reads an acquire test value.
            std::uint32_t LoadAcquire() const noexcept {
                return _value.load(std::memory_order_acquire);
            }

            /// Writes a relaxed test value.
            void StoreRelaxed(std::uint32_t value) noexcept {
                _value.store(
                    value,
                    std::memory_order_relaxed
                );
            }

            /// Writes a released test value.
            void StoreRelease(std::uint32_t value) noexcept {
                _value.store(
                    value,
                    std::memory_order_release
                );
            }

            /// Performs the existing base compare/exchange contract.
            bool CompareExchangeAcqRel(
                std::uint32_t& expected,
                std::uint32_t desired
            ) noexcept {
                return _value.compare_exchange_strong(
                    expected,
                    desired,
                    std::memory_order_acq_rel,
                    std::memory_order_acquire
                );
            }


            // Optional operations deliberately altered only in negative builds.

#if EDP_PLATFORM_SC_NEGATIVE_CASE != 7
            /// Supplies the proper strong load except in the selected wrong-type/throwing case.
#if EDP_PLATFORM_SC_NEGATIVE_CASE == 3
            std::uint64_t
#else
            std::uint32_t
#endif
            LoadSequentiallyConsistent() const noexcept(EDP_PLATFORM_SC_NEGATIVE_CASE != 5) {
                return _value.load(std::memory_order_seq_cst);
            }
#endif

#if EDP_PLATFORM_SC_NEGATIVE_CASE != 8
            /// Supplies the proper strong store except in the selected wrong-type/throwing case.
#if EDP_PLATFORM_SC_NEGATIVE_CASE == 4
            std::uint32_t
#else
            void
#endif
            StoreSequentiallyConsistent(
                std::uint32_t value
            ) noexcept(EDP_PLATFORM_SC_NEGATIVE_CASE != 6) {
                _value.store(
                    value,
                    std::memory_order_seq_cst
                );
#if EDP_PLATFORM_SC_NEGATIVE_CASE == 4
                return value;
#endif
            }
#endif

    };


    /// Base-only provider remains valid without advertising the optional property.
    struct BaseOnlyProvider final : Framework::Provider<
        Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                AtomicWord32,
                Framework::PropertyValue<LockFree, true>,
                Framework::PropertyValue<AtomicWordStorageBytes, 4U>
            >
        >
    > {

        // Provider word identity.

        /// The same four-byte fixture used to exercise compatibility.
        using Word = ShapeWord;

    };


    /// Optional provider whose metadata varies for one targeted negative test.
    struct StrongProvider final : Framework::Provider<
        Platform::Domain,
        Framework::Offers<
            Framework::Offer<
                AtomicWord32,
                Framework::PropertyValue<LockFree, true>,
                Framework::PropertyValue<AtomicWordStorageBytes, 4U>
#if EDP_PLATFORM_SC_NEGATIVE_CASE != 1
                ,
                Framework::PropertyValue<
                    SupportsSequentiallyConsistentLoadStore,
                    EDP_PLATFORM_SC_NEGATIVE_CASE != 2
                >
#endif
            >
        >
    > {

        // Provider word identity.

        /// Native storage remains four bytes in every shape test.
        using Word = ShapeWord;

    };


    static_assert(
        sizeof(Detail::AtomicWord32ProviderTraits<BaseOnlyProvider>) > 0U,
        "The original base contract must remain valid without optional metadata"
    );

    static_assert(
        sizeof(Detail::SequentiallyConsistentAtomicWord32ProviderTraits<StrongProvider>) > 0U,
        "The optional contract must validate metadata, types and exception guarantees"
    );

} // ESPressio::Platform::Concurrency::Tests


/// Runs only the positive fixture; negative modes must fail before linking.
int main() {
    ESPressio::Platform::Concurrency::Tests::ShapeWord word;
#if EDP_PLATFORM_SC_NEGATIVE_CASE == 0
    word.StoreSequentiallyConsistent(42U);
    return word.LoadSequentiallyConsistent() == 42U ? 0 : 1;
#else
    static_cast<void>(word);
    return 0;
#endif
}
