cmake_minimum_required(VERSION 3.8)
project(crc64)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_PIC ON CACHE BOOL "enable X functionality" FORCE)
set(CMAKE_CXX_FLAGS "-march=native")
add_subdirectory(cpu_features)
include_directories(cpu_features/include)
include_directories(include)


add_executable(crc64_run main.cpp)
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

target_link_libraries(crc64_run crc64)