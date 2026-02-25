#pragma once


#include <cstddef>
#include <string>


namespace booking::domain {
    using SessionId = std::string;
    using BookingId = std::string;

    struct SeatPos {
        int row = 0;
        int col = 0;
    };

    constexpr bool operator==(const SeatPos& a, const SeatPos& b) noexcept {
        return a.row == b.row && a.col == b.col;
    }

    struct SeatPosHash {
        std::size_t operator()(const SeatPos& p) const noexcept {
            return (static_cast<std::size_t>(p.row) << 32) ^ static_cast<std::size_t>(p.col);
        }
    };

    enum class SeatType { Regular, Vip };

} // namespace booking::domain