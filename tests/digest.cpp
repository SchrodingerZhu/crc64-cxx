//
// Created by schrodinger on 6/30/21.
//

#include <crc64.hpp>
#include <cstring>
#include <gtest/gtest.h>
#include <iomanip>
#include <random>
#include <vector>

template<class A, class B>
using Cases = std::vector<std::pair<A, B>>;

TEST(Digest, Simple)
{
  auto cases = Cases<std::vector<char>, uint64_t>{
    {{}, 0},
    {std::vector<char>(1, '@'), 0x7b1b'8ab9'8fa4'b8f8},
    {std::vector<char>{'1', '\x97'}, 0xfeb8'f7a1'ae3b'9bd4},
    {std::vector<char>{'M', '\"', '\xdf'}, 0xc016'0ce8'dd46'74d3},
    {std::vector<char>{'l', '\xcd', '\x13', '\xd7'}, 0x5c60'a6af'8299'6ea8},
    {std::vector<char>(32, 0), 0xc95a'f861'7cd5'330c},
    {std::vector<char>(32, -1), 0xe95d'ce9e'faa0'9acf},
    {{'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
      '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
      '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
      '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F'},
     0x7fe5'71a5'8708'4d10},
    {std::vector<char>(1024, 0), 0xc378'6397'2069'270c}};
  for (auto [x, y] : cases)
  {
    auto simd = crc64::Digest();
    simd.update(x.data(), x.size());
    ASSERT_EQ(simd.checksum(), y);
    auto table = crc64::Digest(crc64::Mode::Table);
    table.update(x.data(), x.size());
    ASSERT_EQ(table.checksum(), y);
  }
}

TEST(Digest, Random)
{
  auto dev = std::random_device{};
  auto seed = dev();
  auto eng = std::mt19937_64{seed};
  auto dist = std::uniform_int_distribution<char>{};
  for (auto i = 0; i < 1000; ++i)
  {
    std::vector<char> data;
    auto length = 10000 + dist(eng) % 1'0000'0000;
    data.reserve(length);
    for (auto t = 0; t < length; ++t)
    {
      data[i] = dist(eng);
    }
    auto simd = crc64::Digest();
    auto table = crc64::Digest(crc64::Mode::Table);
    simd.update(data.data(), data.size());
    table.update(data.data(), data.size());
    ASSERT_EQ(table.checksum(), simd.checksum())
      << "random engine seeded with: 0x" << std::hex << seed << std::endl;
  }
}

TEST(Digest, Alignment)
{
  auto dev = std::random_device{};
  auto seed = dev();
  auto eng = std::mt19937_64{seed};
  auto dist = std::uniform_int_distribution<char>{};
  auto data = std::vector<char>(8192);
  for (auto& i : data)
  {
    i = dist(eng);
  }
  auto digest = crc64::Digest{};
  digest.update(data.data(), data.size());
  auto initial = digest.checksum();
  auto storage =
    reinterpret_cast<char*>(::operator new (8192 * 2, std::align_val_t{1024}));
  for (auto align = 1; align <= 600; ++align)
  {
    std::memcpy(storage + align, data.data(), data.size());
    auto inner = crc64::Digest{};
    inner.update(storage + align, data.size());
    ASSERT_EQ( // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)
      inner.checksum(),
      initial)
      << "random engine seeded with: 0x" << std::hex << seed << std::endl;
  }
  ::operator delete (storage, std::align_val_t{1024});
}

TEST(Digest, Consection)
{
  auto dev = std::random_device{};
  auto seed = dev();
  auto eng = std::mt19937_64{seed};
  auto dist = std::uniform_int_distribution<char>{};
  auto data = std::vector<char>(65536);
  auto digest = crc64::Digest{};
  auto a = crc64::Digest{crc64::Mode::Table};
  auto b = crc64::Digest{crc64::Mode::Auto};
  for (auto& i : data)
  {
    i = dist(eng);
  }
  auto acc = 0;
  for (auto i = 1; acc + i <= data.size(); acc += i, i <<= 1)
  {
    a.update(data.data() + acc, i);
    b.update(data.data() + acc, i);
    ASSERT_EQ(a.checksum(), b.checksum())
      << "random engine seeded with: 0x" << std::hex << seed << std::endl;
  }
}