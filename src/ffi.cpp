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
  crc64_digest_t crc64_create_digest(bool enable_simd)
  {
    crc64_digest_t result;
    if (enable_simd && crc64::FAST_CRC64_SUPPORT)
    {
      result.update_fn = crc64::detail::update_fast;
    }
    else
    {
      result.update_fn = crc64::detail::update_table;
    }
    result.state = ~0ull;
    return result;
  }

  void crc64_write(crc64_digest_t current, const void* src, size_t length)
  {
    current.state = current.update_fn(current.state, src, length);
  }

  crc64_t crc64_checksum(crc64_digest_t current)
  {
    return ~current.state;
  }
}