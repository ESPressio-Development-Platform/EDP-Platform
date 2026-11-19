# Atomic ordering demonstration


## Optional stronger ordering

The three source forms demonstrate stronger word operations alongside their original base behavior. Select matching Platform and Portable feature/profiling-v1 sources; main lacks the extension. PlatformIO manifests use GitHub URLs. The narrow AtomicWord32 entry point avoids unrelated synchronization constraints.

For Arduino IDE, install the selected Platform, Portable, System and Portable's declared Memory/transitive package dependencies; choose ESP32 and C++20. The source runs once at startup. It is a usage/build demonstration, not physical capture-shutdown/overhead qualification.


## Current ESP32 qualification limit

The stock Arduino-ESP32 3.3.12 demo build rejects the Portable provider under its actual SDK/compiler flags: std::atomic<uint32_t>::is_always_lock_free is false. The existing mandatory guard remains intact. These Arduino forms demonstrate the API source but are not an enabled/qualified ESP32 runtime profile on that SDK. Select a qualified provider/toolchain; native Platform support and the complete Profiling lifecycle are separate work. Bare compiler feasibility without the real SDK flags is insufficient evidence.
