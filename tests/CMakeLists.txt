cmake_minimum_required(VERSION 3.16)

project(EDPPlatformTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
