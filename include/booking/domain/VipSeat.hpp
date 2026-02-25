#pragma once

#include "Seat.hpp"

namespace booking::domain {

    class VipSeat final : public Seat {
    public:
        using Seat::Seat;
        SeatType type() const noexcept override { return SeatType::Vip; }
    };

}