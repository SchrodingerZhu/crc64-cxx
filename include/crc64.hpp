//
// Created by schrodinger on 6/29/21.
//

#ifndef CRC64_CRC64_HPP
#define CRC64_CRC64_HPP

#include <crc64/arch.hpp>

namespace crc64
{
  enum class Mode
  {
    Auto,
    SIMD_128,
#if defined(__x86_64) || defined(__x86_64__)
    SIMD_256,
    SIMD_512,
#endif
    Table
  };
  class Digest
  {
  public:
    explicit Digest(Mode mode = Mode::Auto)
    {
#if defined(__x86_64) || defined(__x86_64__)
      if (
        (mode == Mode::Auto && VPCLMULQDQ_AVX512_CRC64_SUPPORT) ||
        mode == Mode::SIMD_512)
      {
        update_fn = [](uint64_t _state, const void* _src, size_t _length) {
          return crc64::detail::update_fast<512>(
            crc64::detail::update_vpclmulqdq_avx512, _state, _src, _length);
        };
      }
      else if (
        (mode == Mode::Auto && VPCLMULQDQ_AVX2_CRC64_SUPPORT) ||
        mode == Mode::SIMD_256)
      {
        update_fn = [](uint64_t _state, const void* _src, size_t _length) {
          return crc64::detail::update_fast<256>(
            crc64::detail::update_vpclmulqdq_avx2, _state, _src, _length);
        };
      }
      else
#endif
        if (
          (mode == Mode::Auto && FAST_CRC64_SUPPORT) || mode == Mode::SIMD_128)
      {
        update_fn = [](uint64_t _state, const void* _src, size_t _length) {
          return crc64::detail::update_fast(
            crc64::detail::update_simd, _state, _src, _length);
        };
      }
      else // NOLINT(readability-misleading-indentation)
      {
        update_fn = detail::update_table;
      }
    };

    void write(const void* src, size_t length)
    {
      state = update_fn(state, src, length);
    }

    [[nodiscard]] uint64_t checksum() const
    {
      return ~state;
    }

  private:
    uint64_t (*update_fn)(uint64_t, const void*, size_t) = nullptr;

    uint64_t state = ~0ull;
  };
} // namespace crc64

#endif // CRC64_CRC64_HPP
