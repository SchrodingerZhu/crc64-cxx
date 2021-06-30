# crc64-cxx
SIMD CRC64 for C++

## Benchmark

### Clang-12 (x86_64)
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

### GCC-11 (x86_64)

```
2021-06-30T13:39:18+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.77, 0.72, 0.66
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd        80.0 ns         80.0 ns      7964723 bytes_per_second=11.9251G/s
benchmark_crc64_long_simd     60302869 ns     60230683 ns           12 bytes_per_second=16.6028G/s
benchmark_crc64_fixed_table        307 ns          307 ns      2275141 bytes_per_second=3.10646G/s
benchmark_crc64_long_table   354743824 ns    354378936 ns            2 bytes_per_second=2.82184G/s
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