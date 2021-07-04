//
// Created by schrodinger on 7/4/21.
//
#include <crc64.hpp>
#include <crc64/arch/x86.hpp>

namespace
{
  const cpu_features::X86Features _features = // NOLINT(cert-err58-cpp)
    cpu_features::GetX86Info().features;
} // namespace

namespace crc64
{
  const bool VPCLMULQDQ_AVX2_CRC64_SUPPORT =
    _features.avx2 && _features.vpclmulqdq;

  namespace detail
  {
    using avx256_t = __m256i;

    template<size_t N>
    using Slice = std::array<avx256_t, N>;

    uint64_t
    update_vpclmulqdq_avx2(uint64_t state, const void* src, size_t length)
    {
      if (length == 0)
        return state;

      const auto* ptr =
        reinterpret_cast<const avx256_t*>(__builtin_assume_aligned(src, 256));

      auto load_slice = [](const avx256_t* address) -> Slice<4> {
        return {
          _mm256_load_si256(address),
          _mm256_load_si256(address + 1),
          _mm256_load_si256(address + 2),
          _mm256_load_si256(address + 3)};
      };

      auto x = load_slice(ptr);
      ptr += 4;
      x[0] = _mm256_xor_si256(
        x[0], _mm256_set_epi64x(0, 0, 0, static_cast<int64_t>(state)));

      auto coeff = _mm256_set_epi64x(
        static_cast<int64_t>(K_1023),
        static_cast<int64_t>(K_1087),
        static_cast<int64_t>(K_1023),
        static_cast<int64_t>(K_1087));

      auto fold = [](avx256_t a, avx256_t b) -> avx256_t {
        auto h = _mm256_clmulepi64_epi128(a, b, 0x11);
        auto l = _mm256_clmulepi64_epi128(a, b, 0x00);
        return _mm256_xor_si256(h, l);
      };

      {
        auto chunk = load_slice(ptr);
        for (size_t j = 0; j < 4; ++j)
        {
          auto folded = fold(x[j], coeff);
          x[j] = _mm256_xor_si256(chunk[j], folded);
        }
        ptr += 4;
      }

      for (size_t i = 256; i < length; i += 256, ptr += 8)
      {
        Slice<4> chunk[2] = {load_slice(ptr + 0), load_slice(ptr + 4)};
        for (size_t j = 0; j < 4; ++j)
        {
          {
            auto folded = fold(x[j], coeff);
            x[j] = _mm256_xor_si256(chunk[0][j], folded);
          }
          {
            auto folded = fold(x[j], coeff);
            x[j] = _mm256_xor_si256(chunk[1][j], folded);
          }
        }
      }

      std::array<SIMD, 8> y = {
        SIMD{_mm256_extracti128_si256(x[0], 0)},
        SIMD{_mm256_extracti128_si256(x[0], 1)},
        SIMD{_mm256_extracti128_si256(x[1], 0)},
        SIMD{_mm256_extracti128_si256(x[1], 1)},
        SIMD{_mm256_extracti128_si256(x[2], 0)},
        SIMD{_mm256_extracti128_si256(x[2], 1)},
        SIMD{_mm256_extracti128_si256(x[3], 0)},
        SIMD{_mm256_extracti128_si256(x[3], 1)},
      };

      _mm256_zeroupper(); // ymm, xmm transition

      std::array<SIMD, 7> coeffs = {
        SIMD{K_895, K_959}, // fold by distance of 112 bytes
        SIMD{K_767, K_831}, // fold by distance of 96 bytes
        SIMD{K_639, K_703}, // fold by distance of 80 bytes
        SIMD{K_511, K_575}, // fold by distance of 64 bytes
        SIMD{K_383, K_447}, // fold by distance of 48 bytes
        SIMD{K_255, K_319}, // fold by distance of 32 bytes
        SIMD{K_127, K_191} // fold by distance of 16 bytes
      };

      auto acc = y[7];
      for (size_t i = 0; i < 7; ++i)
      {
        acc ^= y[i].fold16(coeffs[i]);
      }

      return acc.fold8(K_127).barrett(POLY, MU);
    }
  } // namespace detail

} // namespace crc64
