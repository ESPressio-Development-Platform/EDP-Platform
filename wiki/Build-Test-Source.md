# Build, Test and Source Map

C++20 is required. `src/` contains contracts only; concrete implementations belong in Arduino, ESP-IDF, FreeRTOS or Portable provider repositories according to the API that actually owns the implementation. Tests protect contract shape, type properties, timeout/result vocabulary and snapshot semantics.
