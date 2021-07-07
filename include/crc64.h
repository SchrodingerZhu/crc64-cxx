//
// Created by schrodinger on 7/1/21.
//

#ifndef CRC64_CRC64_H
#define CRC64_CRC64_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <crc64_config.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef enum // NOLINT(modernize-use-using)
  {
    CRC64_MODE_AUTO,
    CRC64_MODE_SIMD_128,
#ifdef CRC64_VPCLMULQDQ_SUPPORT
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

  void crc64_update(crc64_digest_t* current, const void* src, size_t length);

  crc64_t crc64_checksum(const crc64_digest_t* current);

#ifdef __cplusplus
}
#endif

#endif // CRC64_CRC64_H
