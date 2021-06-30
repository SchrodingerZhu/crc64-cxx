cmake_minimum_required(VERSION 3.8)
project(crc64)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_PIC ON CACHE BOOL "Needed to require CPU feature PIC build" FORCE)
add_subdirectory(cpu_features)
include_directories(cpu_features/include)
include_directories(include)

add_library(crc64 INTERFACE)
target_link_libraries(crc64 INTERFACE)

set(CRC64_ADDITIONAL_FLAGS -funroll-loops)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CRC64_ADDITIONAL_FLAGS
            ${CRC64_ADDITIONAL_FLAGS}
            -fprefetch-loop-arrays
            -fvariable-expansion-in-unroller)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    add_library(crc64-x86 STATIC src/simd.cpp)
    target_compile_options(crc64-x86 PRIVATE
            -msse2 -msse4.1 -mpclmul ${CRC64_ADDITIONAL_FLAGS})
    target_link_libraries(crc64 INTERFACE crc64-x86 cpu_features)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64)|(ARM64)|(aarch64)|(AARCH64)")
    add_library(crc64-aarch64 STATIC src/simd.cpp)
    target_compile_options(crc64-aarch64 PRIVATE
            -march=armv8-a+simd+fp+crypto ${CRC64_ADDITIONAL_FLAGS})
    target_link_libraries(crc64 INTERFACE crc64-aarch64 cpu_features)
endif()

option(CRC64_ENABLE_TESTS "Enable CRC64 Tests (Require GTest)" ON)
option(CRC64_ENABLE_BENCHES "Enable CRC64 Benchmark (Require Benchmark)" ON)

if (CRC64_ENABLE_TESTS)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    file(GLOB CRC64_TEST_SRC tests/*.cpp)
    add_executable(crc64-test ${CRC64_TEST_SRC})
    target_include_directories(crc64-test  SYSTEM PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(crc64-test ${GTEST_BOTH_LIBRARIES} Threads::Threads crc64)
    target_compile_options(crc64-test PRIVATE -march=native -fno-inline -Og -g3)
    enable_testing(gtest crc64-test)
endif()


if (CRC64_ENABLE_BENCHES)
    find_package(benchmark REQUIRED)
    file(GLOB CRC64_TEST_SRC benches/*.cpp)
    add_executable(crc64-bench ${CRC64_TEST_SRC})
    target_compile_options(crc64-bench PRIVATE -march=native)
    target_link_libraries(crc64-bench crc64 benchmark::benchmark)
endif()
