#include <crc64.hpp>

#if defined(__x86_64) || defined(__x86_64__)

#include <crc64/arch/x86.hpp>

namespace {
    const
    cpu_features::X86Features _features = // NOLINT(cert-err58-cpp)
            cpu_features::GetX86Info().features;
}

const bool crc64::FAST_CRC64_SUPPORT =
        _features.pclmulqdq
        && _features.sse2
        && _features.sse4_1;

#elif defined(__aarch64__) || defined(_M_ARM64) \
 || defined(__arm64) || defined(__arm64__)

#include <crc64/arch/aarch64.hpp>


namespace {
    const
    cpu_features::Aarch64Features _features = // NOLINT(cert-err58-cpp)
        cpu_features::GetAarch64Info().features;
}

const bool FAST_CRC64_SUPPORT = detail::_features.pmull
        && detail::_features.asimd;

#endif

namespace crc64::detail {
    template<size_t N>
    using Slice = std::array<SIMD, N>;

    uint64_t update_simd(uint64_t state, const void *src, size_t length) {

        const auto *ptr =
                reinterpret_cast<const SIMD *>(__builtin_assume_aligned(src, 128));

        auto load_slice = [](const SIMD *address) -> Slice<8> {
            return {
                    SIMD::aligned(address),
                    SIMD::aligned(address + 1),
                    SIMD::aligned(address + 2),
                    SIMD::aligned(address + 3),
                    SIMD::aligned(address + 4),
                    SIMD::aligned(address + 5),
                    SIMD::aligned(address + 6),
                    SIMD::aligned(address + 7),
            };
        };

        auto x = load_slice(ptr);
        ptr += 8;
        x[0] ^= SIMD{0, state};

        auto coeff = SIMD{K_1023, K_1087};

        for (size_t i = 8; i < length; i += 8, ptr += 8) {
            auto chunk = load_slice(ptr);
            for (size_t j = 0; j < 8; ++j) {
                x[j] = chunk[j] ^ x[j].fold16(coeff);
            }
        }

        Slice<7> coeffs = {
                SIMD{K_895, K_959}, // fold by distance of 112 bytes
                SIMD{K_767, K_831}, // fold by distance of 96 bytes
                SIMD{K_639, K_703}, // fold by distance of 80 bytes
                SIMD{K_511, K_575}, // fold by distance of 64 bytes
                SIMD{K_383, K_447}, // fold by distance of 48 bytes
                SIMD{K_255, K_319}, // fold by distance of 32 bytes
                SIMD{K_127, K_191} // fold by distance of 16 bytes
        };

        auto acc = x[7];
        for (size_t i = 0; i < 7; ++i) {
            acc ^= x[i].fold16(coeffs[i]);
        }

        return acc.fold8(K_127).barrett(POLY, MU);

    }
}