//
// Created by schrodinger on 6/29/21.
//

#ifndef CRC64_AARCH64_HPP
#define CRC64_AARCH64_HPP

#include <cpuinfo_aarch64.h>
#include <arm_neon.h>
#include <cstdint>
#include <cstring>

namespace crc64 {
    namespace detail {
        using simd_t = uint8x16_t;
    }
    class SIMD {
    public:

        SIMD(uint64_t high, uint64_t low) noexcept;

        SIMD fold16(SIMD coeff) const noexcept;

        SIMD fold8(uint64_t coeff) const noexcept;

        uint64_t barrett(uint64_t poly, uint64_t mu) const noexcept;

        SIMD bitxor(SIMD that) const noexcept;

        static SIMD aligned(const void * address) noexcept;

        SIMD& operator^=(const SIMD& that) noexcept;

        SIMD operator^(const SIMD& that) const noexcept;

        bool operator==(const SIMD &that) const noexcept;

    private:

        detail::simd_t _inner {};

        explicit SIMD(detail::simd_t _inner) noexcept;

        using Poly64Pair = std::pair<poly64_t, poly64_t>;

        static SIMD from_mul(poly64_t a, poly64_t b);

        Poly64Pair into_poly64pair() const noexcept;

        poly64_t high64() const noexcept;

        poly64_t low64() const noexcept;

    };

    inline SIMD::SIMD(uint64_t high, uint64_t low) noexcept {
        _inner = vcombine_u8(vcreate_u8(low), vcreate_u8(high));
    }

    inline SIMD SIMD::bitxor(SIMD that) const noexcept {
        return SIMD{veorq_u8(_inner, that._inner)};
    }

    inline SIMD SIMD::fold8(uint64_t coeff) const noexcept {
        auto [x0, x1] = into_poly64pair();
        auto h        = SIMD::from_mul(static_cast<poly64_t>(coeff), x0);
        auto l        = SIMD{0, static_cast<uint64_t>(x1)};
        return h.bitxor(l);
    }

    inline SIMD SIMD::fold16(SIMD coeff) const noexcept {
//        auto [x0, x1] = into_poly64pair();
//        auto [c0, c1] = coeff.into_poly64pair();
//        auto h = SIMD::from_mul(c0, x0);
//        auto l = SIMD::from_mul(c1, x1);
        SIMD h(0, 0), l(0, 0);
        asm(
                "pmull %0.1q, %2.1d, %3.1d\n"
                "pmull2 %1.1q, %2.2d, %3.2d"
        : "=&w"(l._inner), "=w"(h._inner)
        : "w"(this->_inner), "w"(coeff._inner)
        );
        return h.bitxor(l);
    }

    inline uint64_t SIMD::barrett(uint64_t poly, uint64_t mu) const noexcept {
        auto t1 = SIMD::from_mul(low64(), static_cast<poly64_t>(mu)).low64();
        auto l  = SIMD::from_mul(t1, static_cast<poly64_t>(mu));
        auto reduced = static_cast<uint64_t>(bitxor(l).high64());
        return reduced ^ static_cast<uint64_t>(t1);
    }

    inline SIMD::SIMD(detail::simd_t _inner) noexcept: _inner(_inner) {}

    inline SIMD &SIMD::operator^=(const SIMD& that) noexcept {
        this->_inner = this->bitxor(that)._inner;
        return *this;
    }

    inline SIMD SIMD::operator^(const SIMD& that) const noexcept {
        return bitxor(that);
    }

    inline bool SIMD::operator==(const SIMD &that) const noexcept {
        return ::memcmp(&_inner, &that._inner, 16) == 0;
    }

    inline SIMD SIMD::aligned(const void *address) noexcept {
        return SIMD { vld1q_u8(reinterpret_cast<const uint8_t *>(address)) };
    }

    inline SIMD SIMD::from_mul(poly64_t a, poly64_t b) noexcept {
        auto mul = vmull_p64(a, b);
        return SIMD(vreinterpretq_u8_p128(mul));
    }

    inline Poly64Pair SIMD::into_poly64pair() const noexcept {
        auto y = vreinterpretq_p64_u8(_inner);
        return {vgetq_lane_p64(y, 0), vgetq_lane_p64(y, 1)};
    }

    inline poly64_t SIMD::high64() const noexcept {
        auto y = vreinterpretq_p64_u8(_inner);
        return vgetq_lane_p64(y, 1);
    }

    inline poly64_t SIMD::low64() const noexcept {
        auto y = vreinterpretq_p64_u8(_inner);
        return vgetq_lane_p64(y, 0);
    }
}

#endif //CRC64_AARCH64_HPP
