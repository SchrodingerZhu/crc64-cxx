# crc64-cxx
SIMD CRC64 for C++

## Benchmark

### GCC-11 (x86_64)
```
2021-06-30T15:35:11+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.78, 0.88, 0.73
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd        78.2 ns         78.2 ns      8873243 bytes_per_second=12.2006G/s
benchmark_crc64_long_simd       135474 ns       135313 ns         5150 bytes_per_second=28.8682G/s
benchmark_crc64_fixed_table        305 ns          305 ns      2313753 bytes_per_second=3.13055G/s
benchmark_crc64_long_table     1436813 ns      1435035 ns          498 bytes_per_second=2.72206G/s
```

### Clang-12 (x86_64)

```
2021-06-30T15:33:55+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 1.00, 0.91, 0.73
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd        78.6 ns         78.6 ns      8748214 bytes_per_second=12.1402G/s
benchmark_crc64_long_simd       128588 ns       128444 ns         5415 bytes_per_second=30.412G/s
benchmark_crc64_fixed_table        415 ns          414 ns      1671464 bytes_per_second=2.30153G/s
benchmark_crc64_long_table     1734823 ns      1732972 ns          403 bytes_per_second=2.25408G/s
```

### GCC-11 (Aarch64)

```
2021-06-30T15:32:49+08:00
Running ./crc64-bench
Run on (96 X 2600 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x96)
  L1 Instruction 64 KiB (x96)
  L2 Unified 512 KiB (x96)
  L3 Unified 49152 KiB (x4)
Load Average: 1.71, 2.52, 2.81
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd         167 ns          167 ns      3077216 bytes_per_second=5.69453G/s
benchmark_crc64_long_simd       423398 ns       423358 ns         1692 bytes_per_second=9.22682G/s
benchmark_crc64_fixed_table        600 ns          600 ns      1166694 bytes_per_second=1.58913G/s
benchmark_crc64_long_table     2467881 ns      2467552 ns          284 bytes_per_second=1.58305G/s
```