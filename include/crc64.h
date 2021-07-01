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

  typedef uint64_t crc64_t; // NOLINT(modernize-use-using)

  typedef struct // NOLINT(modernize-use-using)
  {
    crc64_t (*update_fn)(uint64_t, const void*, size_t);
    crc64_t state;
  } crc64_digest_t;

  crc64_digest_t crc64_create_digest(bool enable_simd);

  void crc64_write(crc64_digest_t current, const void* src, size_t length);

  crc64_t crc64_checksum(crc64_digest_t current);

#ifdef __cplusplus
}
#endif

#endif // CRC64_CRC64_H
