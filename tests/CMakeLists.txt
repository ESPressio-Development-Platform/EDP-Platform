cmake_minimum_required(VERSION 3.16)

project(EDPPlatformTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Normalize caller-supplied sibling paths relative to the repository root.
if(DEFINED EDP_SYSTEM_SOURCE_DIR)
    get_filename_component(
        EDP_SYSTEM_SOURCE_DIR
        "${EDP_SYSTEM_SOURCE_DIR}"
        ABSOLUTE
        BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
endif()



if(NOT DEFINED EDP_SYSTEM_SOURCE_DIR)
    message(FATAL_ERROR "EDP_SYSTEM_SOURCE_DIR must point at an EDP-System checkout")
endif()

add_executable(
    synchronization_contract_tests
    synchronization/SynchronizationContractTests.cpp
)

target_include_directories(
    synchronization_contract_tests
    PRIVATE
        ../src
        ${EDP_SYSTEM_SOURCE_DIR}/src
)

add_executable(
    execution_contract_tests
    execution/ExecutionContractTests.cpp
)

target_include_directories(
    execution_contract_tests
    PRIVATE
        ../src
        ${EDP_SYSTEM_SOURCE_DIR}/src
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        synchronization_contract_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )

    target_compile_options(
        execution_contract_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endif()


add_executable(
    randomness_contract_tests
    randomness/RandomContractTests.cpp
)

target_include_directories(
    randomness_contract_tests
    PRIVATE
        ../src
        ${EDP_SYSTEM_SOURCE_DIR}/src
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        randomness_contract_tests
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
    )
endif()

# H471: retain all original contracts, register the existing concurrency suite and
# the buffer-reuse/rollover regressions, with assertions enabled in Release.
find_package(Threads REQUIRED)
add_executable(concurrency_tests concurrency/ConcurrencyTests.cpp)
add_executable(snapshot_reuse_ordering_tests concurrency/SnapshotReuseOrderingTests.cpp)
foreach(test_name concurrency_tests snapshot_reuse_ordering_tests)
    target_include_directories(${test_name} PRIVATE ../src ${EDP_SYSTEM_SOURCE_DIR}/src)
    target_link_libraries(${test_name} PRIVATE Threads::Threads)
endforeach()
enable_testing()
foreach(test_name synchronization_contract_tests execution_contract_tests
        randomness_contract_tests concurrency_tests snapshot_reuse_ordering_tests)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_name} PRIVATE -Wall -Wextra -Wpedantic -Werror -UNDEBUG)
    endif()
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
