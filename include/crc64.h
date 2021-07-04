//
// Created by schrodinger on 7/1/21.
//

#ifndef CRC64_CRC64_H
#define CRC64_CRC64_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum // NOLINT(modernize-use-using)
  {
    CRC64_MODE_AUTO,
    CRC64_MODE_SIMD_128,
#if defined(__x86_64) || defined(__x86_64__)
    CRC64_MODE_SIMD_256,
    CRC64_MODE_SIMD_512,
#endif
    CRC64_MODE_TABLE,
  } crc64_mode_t;

  typedef uint64_t crc64_t; // NOLINT(modernize-use-using)

  typedef struct // NOLINT(modernize-use-using)
  {
    crc64_t (*update_fn)(uint64_t, const void*, size_t);
    crc64_t state;
  } crc64_digest_t;

  crc64_digest_t crc64_create_digest(crc64_mode_t mode);

  void crc64_update(crc64_digest_t current, const void* src, size_t length);

  crc64_t crc64_checksum(crc64_digest_t current);

#ifdef __cplusplus
}
#endif

#endif // CRC64_CRC64_H
