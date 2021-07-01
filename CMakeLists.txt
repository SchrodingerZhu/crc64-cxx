cmake_minimum_required(VERSION 3.8)
project(crc64)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_PIC ON CACHE BOOL "Needed to require CPU feature PIC build" FORCE)
add_subdirectory(cpu_features)
include_directories(cpu_features/include)
include_directories(include)

add_library(crc64 INTERFACE)
target_link_libraries(crc64 INTERFACE)

option(CRC64_ENABLE_TESTS "Enable CRC64 Tests (Require GTest)" ON)
option(CRC64_ENABLE_BENCHES "Enable CRC64 Benchmark (Require Benchmark)" ON)
option(CRC64_ENABLE_CAPI "Enable CRC64 CAPI" ON)
option(CRC64_DISABLE_WERROR "Disable strict mode" OFF)
# Premature optimization is the root of all evil -- Donald Knuth
option(CRC64_ENABLE_UNROLL "Enable stronger unrolling" OFF)


if (CRC64_ENABLE_CAPI)
    set(CRC64_CAPI_SRC src/ffi.cpp)
else()
    set(CRC64_CAPI_SRC "")
endif ()

if(NOT CRC64_DISABLE_WERROR)
    set(CRC64_WARNING_FLAGS
            "-Werror"
            "-Wall"
            "-Wextra"
            "-Wpedantic"
            "-Wshadow"
            "-Wcast-align"
            "-Wunused"
            "-Wconversion"
            "-Wsign-conversion"
            "-Wnull-dereference"
            "-Wdouble-promotion"
            "-Wformat=2")
else()
    set(CRC64_WARNING_FLAGS "")
endif()

if (CRC64_ENABLE_UNROLL)
    set(CRC64_ADDITIONAL_FLAGS -funroll-loops)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CRC64_ADDITIONAL_FLAGS
                ${CRC64_ADDITIONAL_FLAGS}
                -fprefetch-loop-arrays
                -fvariable-expansion-in-unroller)
    endif()
else()
    set(CRC64_ADDITIONAL_FLAGS "")
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    add_library(crc64-x86 STATIC src/simd.cpp ${CRC64_CAPI_SRC})
    target_compile_options(crc64-x86 PRIVATE
            -msse2 -msse4.1 -mpclmul
            ${CRC64_ADDITIONAL_FLAGS}
            ${CRC64_WARNING_FLAGS})
    target_link_libraries(crc64 INTERFACE crc64-x86 cpu_features)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64)|(ARM64)|(aarch64)|(AARCH64)")
    add_library(crc64-aarch64 STATIC src/simd.cpp ${CRC64_CAPI_SRC})
    target_compile_options(crc64-aarch64 PRIVATE
            -march=armv8-a+simd+fp+crypto
            ${CRC64_ADDITIONAL_FLAGS}
            ${CRC64_WARNING_FLAGS})
    target_link_libraries(crc64 INTERFACE crc64-aarch64 cpu_features)
endif()

if (CRC64_ENABLE_TESTS)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    file(GLOB CRC64_TEST_SRC tests/*.cpp)
    add_executable(crc64-test ${CRC64_TEST_SRC})
    target_include_directories(crc64-test  SYSTEM PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(crc64-test ${GTEST_BOTH_LIBRARIES} Threads::Threads crc64)
    target_compile_options(crc64-test PRIVATE -march=native -fno-inline -Og -g3)
    enable_testing()
    add_test(gtest crc64-test)
endif()


if (CRC64_ENABLE_BENCHES)
    find_package(benchmark REQUIRED)
    file(GLOB CRC64_TEST_SRC benches/*.cpp)
    add_executable(crc64-bench ${CRC64_TEST_SRC})
    target_compile_options(crc64-bench PRIVATE -march=native)
    target_link_libraries(crc64-bench crc64 benchmark::benchmark)
endif()


file(GLOB CRC64_FMT_FILES
        benches/*.cpp
        tests/*.cpp
        src/*.cpp
        include/*.hpp
        include/*.h
        include/crc64/*.hpp
        include/crc64/*.h
        )

add_custom_target(
        format
        COMMAND clang-format
        -i
        ${CRC64_FMT_FILES})
