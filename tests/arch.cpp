#include <gtest/gtest.h>

#if defined(__x86_64) || defined(__x86_64__)

#include <crc64/arch/x86.hpp>

#elif defined(__aarch64__) || defined(_M_ARM64) \
 || defined(__arm64) || defined(__arm64__)
#include <crc64/arch/aarch64.hpp>
#endif

using namespace crc64::detail;
using namespace crc64;

TEST(Arch, SizeAlign) {
    ASSERT_EQ(alignof(SIMD), 16);
    ASSERT_EQ(sizeof(SIMD), 16);
}

TEST(Arch, Equality) {
    auto x = SIMD{0xd7c8'11cf'e5c5'c792, 0x86e6'5c36'e68b'4804};
    auto y = SIMD{0xd7c8'11cf'e5c5'c792, 0x86e6'5c36'e68b'4804};
    auto z = SIMD{0xfa3e'0099'cd5e'd60d, 0xad71'9ee6'57d1'498e};
    ASSERT_EQ(x, y);
    ASSERT_FALSE(x == z);
}

TEST(Arch, Xor) {
    auto x = SIMD{0xe450'87f9'b031'0d47, 0x3d72'e92a'96c7'4c63};
    auto y = SIMD{0x7ed8'ae0a'dfbd'89c0, 0x1c9b'dfaa'953e'0ef4};
    auto z = x ^ y;
    ASSERT_EQ(z, SIMD (0x9a88'29f3'6f8c'8487, 0x21e9'3680'03f9'4297));
    z ^= SIMD {0x57a2'0f44'c005'b2ea, 0x7056'bde9'9303'aa51};
    ASSERT_EQ(z, SIMD (0xcd2a'26b7'af89'366d, 0x51bf'8b69'90fa'e8c6));
}

TEST(Arch, Fold16) {
    auto x = SIMD { 0xb5f1'2590'5645'0b6c, 0x333a'2c49'c361'9e21 };
    auto f = x.fold16(SIMD(0xbecc'9dd9'038f'c366, 0x5ba9'365b'e2e9'5bf5));
    ASSERT_EQ(f, SIMD(0x4f55'42df'ef35'1810, 0x0c03'5bd6'70fc'5abd));
}

TEST(Arch, Fold8) {
    auto x = SIMD(0x60c0'b48f'4a92'2003, 0x203c'f7bc'ad34'103b);
    auto f = x.fold8(0x3e90'3688'ea71'f472);
    ASSERT_EQ(f, SIMD(0x07d7'2761'4d16'56db, 0x2bc0'ed8a'a341'7665));
}

TEST(Arch, Barrett) {
    auto x = SIMD(0x2606'e582'3406'9bae, 0x76cc'1105'0fef'6d68);
    auto b = x.barrett(0x435d'0f79'19a6'1445, 0x5817'6272'f8fa'b8d5);
    ASSERT_EQ(b, 0x5e4d'0253'942a'd95d);
}