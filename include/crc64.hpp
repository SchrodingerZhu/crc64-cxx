//
// Created by schrodinger on 6/29/21.
//

#ifndef CRC64_CRC64_HPP
#define CRC64_CRC64_HPP

#include <crc64/arch.hpp>

namespace crc64
{
  class Digest
  {
  public:
    explicit Digest(bool simd = true)
    {
#if defined(__x86_64) || defined(__x86_64__)
      if (simd && VPCLMULQDQ_CRC64_SUPPORT)
      {
        update_fn = [](uint64_t _state, const void* _src, size_t _length) {
          return crc64::detail::update_fast<512>(
            crc64::detail::update_vpclmulqdq, _state, _src, _length);
        };
      }
      else
#endif
        if (simd && FAST_CRC64_SUPPORT)
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
