//
// Created by schrodinger on 6/29/21.
//

#ifndef CRC64_ARCH_HPP
#define CRC64_ARCH_HPP

#include "table.hpp"

#include <crc64_config.h>
namespace crc64
{
  extern const bool FAST_CRC64_SUPPORT;

#ifdef CRC64_VPCLMULQDQ_SUPPORT
  extern const bool VPCLMULQDQ_AVX512_CRC64_SUPPORT;
  extern const bool VPCLMULQDQ_AVX2_CRC64_SUPPORT;
#endif

  namespace detail
  {
    extern uint64_t update_simd(uint64_t state, const void* src, size_t length);

#ifdef CRC64_VPCLMULQDQ_SUPPORT
    extern uint64_t
    update_vpclmulqdq_avx512(uint64_t state, const void* src, size_t length);

    extern uint64_t
    update_vpclmulqdq_avx2(uint64_t state, const void* src, size_t length);
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
      if constexpr (ALIGN > 128)
      {
        state = update_fast<128>(update_simd, state, ptr, offset);
        state = func(state, ptr + offset, middle);
        state =
          update_fast<128>(update_simd, state, ptr + offset + middle, suffix);
      }
      else
      {
        state = update_table(state, ptr, offset);
        state = func(state, ptr + offset, middle);
        state = update_table(state, ptr + offset + middle, suffix);
      }
      return state;
    }

  } // namespace detail
} // namespace crc64

#endif // CRC64_ARCH_HPP
