//
// Created by schrodinger on 6/29/21.
//

#ifndef CRC64_ARCH_HPP
#define CRC64_ARCH_HPP

#include "table.hpp"

namespace crc64
{
  extern const bool FAST_CRC64_SUPPORT;

#if defined(__x86_64) || defined(__x86_64__)
  extern const bool VPCLMULQDQ_CRC64_SUPPORT;
#endif

  namespace detail
  {
    extern uint64_t update_simd(uint64_t state, const void* src, size_t length);

#if defined(__x86_64) || defined(__x86_64__)
    extern uint64_t
    update_vpclmulqdq(uint64_t state, const void* src, size_t length);
#endif

    template<uintptr_t ALIGN = 128, class Fn>
    static inline uint64_t
    update_fast(Fn func, uint64_t state, const void* src, size_t length)
    {
      static const uintptr_t MASK = ALIGN - 1;

      if (length == 0)
        return state;

      auto offset = (-reinterpret_cast<uintptr_t>(src)) & MASK;

      if (offset >= length)
      {
        return update_table(state, src, length);
      }

      auto suffix = (length - offset) & MASK;
      auto middle = length - offset - suffix;
      const auto* ptr = reinterpret_cast<const uint8_t*>(src);
      state = update_table(state, ptr, offset);
      state = func(state, ptr + offset, middle);
      state = update_table(state, ptr + offset + middle, suffix);
      return state;
    }

  } // namespace detail
} // namespace crc64

#endif // CRC64_ARCH_HPP
