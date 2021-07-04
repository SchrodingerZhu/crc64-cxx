//
// Created by schrodinger on 6/30/21.
//
#include <benchmark/benchmark.h>
#include <crc64.hpp>
#include <random>
#include <vector>

#define UNUSED(x) ((void)(x))

static void benchmark_crc64_fixed_simd(benchmark::State& state)
{
  std::vector<char> data(1024, 48);
  auto d = crc64::Digest{true};
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }
  state.SetBytesProcessed(1024 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_fixed_table(benchmark::State& state)
{
  std::vector<char> data(1024, 48);
  auto d = crc64::Digest{false};
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }
  state.SetBytesProcessed(1024 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_long_simd(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{true};
  std::vector<char> data(4 * 1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }
  state.SetBytesProcessed(
    4 * 1024 * 1024 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_long_table(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{false};
  std::vector<char> data(4 * 1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }

  state.SetBytesProcessed(
    4 * 1024 * 1024 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_varlength_simd(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{true};
  std::vector<char> data(1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    for (auto l = 1; l < 1024 * 1024; l <<= 1)
    {
      d.write(data.data(), l);
    }
    benchmark::DoNotOptimize(d.checksum());
  }

  state.SetBytesProcessed(2097151 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_varlength_table(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{false};
  std::vector<char> data(1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    for (auto l = 1; l <= 1024 * 1024; l <<= 1)
    {
      d.write(data.data(), l);
    }
    benchmark::DoNotOptimize(d.checksum());
  }

  state.SetBytesProcessed(2097151 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_gib_simd(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{true};
  std::vector<char> data(1024 * 1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }
  state.SetBytesProcessed(
    1024 * 1024 * 1024 * static_cast<int64_t>(state.iterations()));
}

static void benchmark_crc64_gib_table(benchmark::State& state)
{
  auto dev = std::random_device{};
  auto eng = std::mt19937_64{dev()};
  auto dist = std::uniform_int_distribution<char>{};
  auto d = crc64::Digest{false};
  std::vector<char> data(1024 * 1024 * 1024);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  for (auto _ : state)
  {
    UNUSED(_);
    d.write(data.data(), data.size());
    benchmark::DoNotOptimize(d.checksum());
  }

  state.SetBytesProcessed(
    1024 * 1024 * 1024 * static_cast<int64_t>(state.iterations()));
}

// Register the function as a benchmark
BENCHMARK(benchmark_crc64_fixed_simd);
BENCHMARK(benchmark_crc64_long_simd);
BENCHMARK(benchmark_crc64_varlength_simd);
BENCHMARK(benchmark_crc64_gib_simd);
BENCHMARK(benchmark_crc64_fixed_table);
BENCHMARK(benchmark_crc64_long_table);
BENCHMARK(benchmark_crc64_varlength_table);
BENCHMARK(benchmark_crc64_gib_table);
BENCHMARK_MAIN();