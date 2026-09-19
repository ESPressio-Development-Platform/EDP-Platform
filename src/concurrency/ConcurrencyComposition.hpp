#pragma once

#include <cstddef>

#include "../platform/PlatformComposition.hpp"

namespace ESPressio::Platform::Concurrency {

    /// Exclusive Platform capability supplying an unsigned 8-bit atomic word implementation.
    struct AtomicWord8 final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Indicates whether an AtomicWord8 provider guarantees operation without a hidden lock.
    struct AtomicWord8LockFree final : Framework::Property<AtomicWord8, bool> {};


    /// Number of bytes occupied by one concrete AtomicWord8 object.
    struct AtomicWord8StorageBytes final : Framework::Property<AtomicWord8, std::size_t> {};


    /// Exclusive Platform capability supplying an unsigned 32-bit atomic word implementation.
    struct AtomicWord32 final : Framework::ExclusiveCapability<Platform::Domain> {};


    /// Indicates whether an AtomicWord32 provider guarantees operation without a hidden lock.
    struct LockFree final : Framework::Property<AtomicWord32, bool> {};


    /// Number of bytes occupied by one concrete AtomicWord32 object.
    struct AtomicWordStorageBytes final : Framework::Property<AtomicWord32, std::size_t> {};

} // ESPressio::Platform::Concurrency
