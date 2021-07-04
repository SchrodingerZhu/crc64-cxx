# crc64-cxx

![ci](https://github.com/schrodingerzhu/crc64-cxx/actions/workflows/cmake.yml/badge.svg)

SIMD CRC64 for C++

## Benchmark

### GCC-11 (x86_64, Intel(R) Xeon(R) E-2176M  CPU @ 2.70GHz)
```
2021-07-01T10:28:14+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.53, 0.69, 0.54
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd            38.2 ns         38.2 ns     18208840 bytes_per_second=24.9727G/s
benchmark_crc64_long_simd           134445 ns       134278 ns         5231 bytes_per_second=29.0908G/s
benchmark_crc64_fixed_table            302 ns          302 ns      2307190 bytes_per_second=3.15697G/s
benchmark_crc64_long_table         1423657 ns      1422090 ns          495 bytes_per_second=2.74684G/s
benchmark_crc64_varlength_simd       34594 ns        34555 ns        20293 bytes_per_second=56.5219G/s
benchmark_crc64_varlength_table     709730 ns       708994 ns          964 bytes_per_second=2.75478G/s
```

### Clang-12 (x86_64, Intel(R) Xeon(R) E-2176M  CPU @ 2.70GHz)

```
2021-07-01T10:30:50+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.58, 0.64, 0.55
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd            37.2 ns         37.2 ns     18740858 bytes_per_second=25.6587G/s
benchmark_crc64_long_simd           127764 ns       127633 ns         5492 bytes_per_second=30.6053G/s
benchmark_crc64_fixed_table            460 ns          460 ns      1526008 bytes_per_second=2.07299G/s
benchmark_crc64_long_table         1814667 ns      1812837 ns          385 bytes_per_second=2.15477G/s
benchmark_crc64_varlength_simd       32863 ns        32831 ns        21361 bytes_per_second=59.491G/s
benchmark_crc64_varlength_table     894499 ns       893570 ns          782 bytes_per_second=2.18575G/s
```

### GCC-11 (Aarch64)

```
2021-07-01T10:28:47+08:00
Running ./crc64-bench
Run on (96 X 2600 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB (x96)
  L1 Instruction 64 KiB (x96)
  L2 Unified 512 KiB (x96)
  L3 Unified 49152 KiB (x4)
Load Average: 2.78, 2.99, 3.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd             170 ns          170 ns      4122286 bytes_per_second=5.6255G/s
benchmark_crc64_long_simd           418681 ns       418576 ns         1725 bytes_per_second=9.33223G/s
benchmark_crc64_fixed_table            600 ns          599 ns      1167795 bytes_per_second=1.59093G/s
benchmark_crc64_long_table         2519544 ns      2518919 ns          279 bytes_per_second=1.55076G/s
benchmark_crc64_varlength_simd       87231 ns        87210 ns         8016 bytes_per_second=22.3957G/s
benchmark_crc64_varlength_table    1259384 ns      1259150 ns          554 bytes_per_second=1.55114G/s
```

## Separate Test for `VPCLMULQDQ`

Runs on `Intel(R) Xeon(R) Platinum 8369B CPU @ 2.70GHz`. Compiled with `gcc-11`.

### 128-bit Only
```
Running ./crc64-bench
Run on (2 X 2700 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x1)
  L1 Instruction 32 KiB (x1)
  L2 Unified 1280 KiB (x1)
  L3 Unified 49152 KiB (x1)
Load Average: 0.23, 0.23, 0.13
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd            92.3 ns         92.2 ns      7586516 bytes_per_second=10.3475G/s
benchmark_crc64_long_simd           193467 ns       193004 ns         3614 bytes_per_second=20.2392G/s
benchmark_crc64_varlength_simd       48320 ns        48261 ns        14500 bytes_per_second=40.47G/s
benchmark_crc64_gib_simd          81279160 ns     81082114 ns            9 bytes_per_second=12.3332G/s
benchmark_crc64_fixed_table            351 ns          350 ns      1998319 bytes_per_second=2.72336G/s
benchmark_crc64_long_table         1450123 ns      1446379 ns          484 bytes_per_second=2.70071G/s
benchmark_crc64_varlength_table     722856 ns       721454 ns          970 bytes_per_second=2.70721G/s
benchmark_crc64_gib_table        382071532 ns    381179563 ns            2 bytes_per_second=2.62344G/s
```

### 512-bit Enabled
```
Running ./crc64-bench
Run on (2 X 2700 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x1)
  L1 Instruction 32 KiB (x1)
  L2 Unified 1280 KiB (x1)
  L3 Unified 49152 KiB (x1)
Load Average: 0.57, 0.35, 0.18
------------------------------------------------------------------------------------------
Benchmark                                Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd             106 ns          106 ns      6584511 bytes_per_second=8.99199G/s
benchmark_crc64_long_simd           129746 ns       129406 ns         5404 bytes_per_second=30.1859G/s
benchmark_crc64_varlength_simd       32707 ns        32664 ns        21427 bytes_per_second=59.7935G/s
benchmark_crc64_gib_simd          66175908 ns     65982478 ns           11 bytes_per_second=15.1555G/s
benchmark_crc64_fixed_table            350 ns          349 ns      2000785 bytes_per_second=2.73026G/s
benchmark_crc64_long_table         1447653 ns      1444234 ns          484 bytes_per_second=2.70472G/s
benchmark_crc64_varlength_table     721965 ns       720647 ns          972 bytes_per_second=2.71024G/s
benchmark_crc64_gib_table        381661762 ns    380784321 ns            2 bytes_per_second=2.62616G/s
```