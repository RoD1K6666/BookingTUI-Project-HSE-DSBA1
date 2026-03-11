#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace booking::domain {

    using SessionId = std::string;
    using BookingId = std::string;
    using HallId = std::string;
    using Money = std::int64_t;

    struct SeatPos {
        int row = 0;
        int col = 0;
    };

    constexpr bool operator==(const SeatPos& a, const SeatPos& b) noexcept {
        return a.row == b.row && a.col == b.col;
    }
    constexpr bool operator!=(const SeatPos& a, const SeatPos& b) noexcept {
        return !(a == b);
    }

    struct SeatPosHash {
        std::size_t operator()(const SeatPos& p) const noexcept {
            std::size_t h1 = std::hash<int>{}(p.row);
            std::size_t h2 = std::hash<int>{}(p.col);
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
        }
    };

    enum class SeatType { Regular, Vip };

}