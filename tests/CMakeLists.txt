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
