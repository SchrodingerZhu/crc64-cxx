# crc64-cxx
SIMD CRC64 for C++

## Benchmark

### Clang-12 (x86_64)
```
2021-06-30T13:41:39+08:00
Running ./crc64-bench
Run on (12 X 4400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 256 KiB (x6)
  L3 Unified 12288 KiB (x1)
Load Average: 0.92, 0.84, 0.72
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------
benchmark_crc64_fixed_simd        78.2 ns         78.2 ns      8825317 bytes_per_second=12.1984G/s
benchmark_crc64_long_simd     54990132 ns     54928533 ns           13 bytes_per_second=18.2055G/s
benchmark_crc64_fixed_table        392 ns          392 ns      1787616 bytes_per_second=2.4334G/s
benchmark_crc64_long_table   459531067 ns    459007866 ns            2 bytes_per_second=2.17861G/s
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