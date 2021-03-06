//
// Created by schrodinger on 7/4/21.
//

#include <crc64.h>
#include <gtest/gtest.h>

template<class A, class B>
using Cases = std::vector<std::pair<A, B>>;

TEST(FFI, Simple)
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
    auto simd = crc64_create_digest(CRC64_MODE_AUTO);
    crc64_update(&simd, x.data(), x.size());
    ASSERT_EQ(crc64_checksum(&simd), y);
    auto table = crc64_create_digest(CRC64_MODE_TABLE);
    crc64_update(&table, x.data(), x.size());
    ASSERT_EQ(crc64_checksum(&table), y);
  }
}