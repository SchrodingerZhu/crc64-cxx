# crc64-cxx
SIMD CRC64 for C++

## Benchmark

### GCC-11 (x86_64)
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

### Clang-12 (x86_64)

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