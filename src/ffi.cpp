#include <crc64.h>
#include <crc64.hpp>
#if defined(__x86_64) || defined(__x86_64__)

#  include <crc64/arch/x86.hpp>

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm64) || \
  defined(__arm64__)

#  include <crc64/arch/aarch64.hpp>
#endif

extern "C"
{
  crc64_digest_t crc64_create_digest(crc64_mode_t mode)
  {
    crc64_digest_t result;
#ifdef CRC64_VPCLMULQDQ_SUPPORT
    if (
      (mode == CRC64_MODE_AUTO && crc64::VPCLMULQDQ_AVX512_CRC64_SUPPORT) ||
      mode == CRC64_MODE_SIMD_512)
    {
      result.update_fn = [](uint64_t _state, const void* _src, size_t _length) {
        return crc64::detail::update_fast<512>(
          crc64::detail::update_vpclmulqdq_avx512, _state, _src, _length);
      };
    }
    else if (
      (mode == CRC64_MODE_AUTO && crc64::VPCLMULQDQ_AVX2_CRC64_SUPPORT) ||
      mode == CRC64_MODE_SIMD_256)
    {
      result.update_fn = [](uint64_t _state, const void* _src, size_t _length) {
        return crc64::detail::update_fast<256>(
          crc64::detail::update_vpclmulqdq_avx2, _state, _src, _length);
      };
    }
    else
#endif
      if (
        (mode == CRC64_MODE_AUTO && crc64::FAST_CRC64_SUPPORT) ||
        mode == CRC64_MODE_SIMD_128)
    {
      result.update_fn = [](uint64_t _state, const void* _src, size_t _length) {
        return crc64::detail::update_fast(
          crc64::detail::update_simd, _state, _src, _length);
      };
    }
    else // NOLINT(readability-misleading-indentation)
    {
      result.update_fn = crc64::detail::update_table;
    }
    result.state = ~0ull;
    return result;
  }

  void crc64_update(crc64_digest_t* current, const void* src, size_t length)
  {
    current->state = current->update_fn(current->state, src, length);
  }

  crc64_t crc64_checksum(const crc64_digest_t* current)
  {
    return ~current->state;
  }
}