#ifndef HPA_2110452_MIN_DOM_SET_BITSET_H
#define HPA_2110452_MIN_DOM_SET_BITSET_H

#include "container/generic.h"
#include "container/array.h"

namespace container {

    template<size_t Nb, typename WordT = int>
    struct bitset_t {
        using byte_t = int8_t;

        inline static constexpr size_t Alignment = sizeof(WordT);
        inline static constexpr size_t NumBytes = (Nb + 8 - 1) / 8;
        inline static constexpr size_t SizeActual = memory::nearest_alignment<byte_t, Alignment>(NumBytes);
        inline static constexpr size_t NumElements = SizeActual / Alignment;

        size_t size_;
        size_t num_bytes_;
        size_t size_actual_;
        size_t num_elements_;
        container::array_t<WordT, NumElements> data = {};

        constexpr bitset_t() : size_{Nb} {
            num_bytes_ = (size_ + 8 - 1) / 8;
            size_actual_ = memory::nearest_alignment<byte_t, Alignment>(num_bytes_);
            num_elements_ = size_actual_ / Alignment;
        }

        explicit constexpr bitset_t(size_t N) : size_{N} {
            num_bytes_ = (size_ + 8 - 1) / 8;
            size_actual_ = memory::nearest_alignment<byte_t, Alignment>(num_bytes_);
            num_elements_ = size_actual_ / Alignment;
        }

        [[nodiscard]] FORCE_INLINE constexpr bool get(size_t index) const {
            size_t idx_word = index / (8 * Alignment);
            size_t idx_bit = index % (8 * Alignment);
            return (data[idx_word] >> idx_bit) & 1;
        }

        FORCE_INLINE constexpr void set(size_t index, bool value) {
            size_t idx_word = index / (8 * Alignment);
            size_t idx_bit = index % (8 * Alignment);

            data[idx_word] &= ~(static_cast<WordT>(1) << idx_bit);
            data[idx_word] |= (static_cast<WordT>(value) << idx_bit);
        }

        FORCE_INLINE constexpr void clear(size_t index) {
            size_t idx_word = index / (8 * Alignment);
            size_t idx_bit = index % (8 * Alignment);

            data[idx_word] &= ~(static_cast<WordT>(1) << idx_bit);
        }

        FORCE_INLINE constexpr void toggle(size_t index) {
            size_t idx_word = index / (8 * Alignment);
            size_t idx_bit = index % (8 * Alignment);

            data[idx_word] ^= (static_cast<WordT>(1) << idx_bit);
        }

        bitset_t &operator&=(const bitset_t &other) {
            for (size_t i = 0; i < NumElements; ++i) {
                data[i] &= other.data[i];
            }
            return *this;
        }

        bitset_t &operator|=(const bitset_t &other) {
            for (size_t i = 0; i < NumElements; ++i) {
                data[i] |= other.data[i];
            }
            return *this;
        }

        bitset_t operator&(const bitset_t &other) {
            bitset_t dst{*this};
            return (dst &= other);
        }

        bitset_t operator|(const bitset_t &other) {
            bitset_t dst{*this};
            return (dst |= other);
        }

        [[nodiscard]] constexpr bool all() const noexcept {
            if (size_ == size_actual_ * 8) {
                for (size_t i = 0; i < num_elements_; ++i) {
                    if (~data[i] != 0) return false;
                }
            } else {
                for (size_t i = 0; i < num_elements_ - 1; ++i) {
                    if (~data[i] != 0) return false;
                }
                for (size_t i = (num_elements_ - 1) * Alignment * 8; i < size_; ++i) {
                    if (!get(i)) return false;
                }
            }

            return true;
        }

        [[nodiscard]] constexpr bool any() const noexcept {
            for (size_t i = 0; i < NumElements; ++i) {
                if (data[i]) return true;
            }

            return false;
        }

        [[nodiscard]] constexpr bool none() const noexcept {
            return !any();
        }

        [[nodiscard]] FORCE_INLINE constexpr types::size_type size() const noexcept {
            return size_;
        }

        [[nodiscard]] FORCE_INLINE constexpr types::size_type capacity() const noexcept {
            return Nb;
        }
    };
}

#endif //HPA_2110452_MIN_DOM_SET_BITSET_H
