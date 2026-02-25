#pragma once

#include "booking/domain/Types.hpp"

namespace booking::domain {

    class Seat {
    public:
        explicit Seat(SeatPos pos) : pos_(pos) {}
        virtual ~Seat() = default;

        SeatPos position() const noexcept { return pos_; }
        virtual SeatType type() const noexcept = 0;

    protected:
        SeatPos pos_;
    };

} // namespace booking::domain