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
      if (simd && FAST_CRC64_SUPPORT)
      {
        update_fn = detail::update_fast;
      }
      else
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
