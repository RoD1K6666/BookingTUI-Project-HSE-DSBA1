#pragma once

#include <vector>
#include <memory>

#include "Seat.hpp"
#include "RegularSeat.hpp"
#include "VipSeat.hpp"
#include "Errors.hpp"

namespace booking::domain {

    class Hall {
    public:
        Hall(int rows, int cols);

        int rows() const noexcept;
        int cols() const noexcept;

        const Seat& seatAt(SeatPos pos) const;
        Seat& seatAt(SeatPos pos);

        void makeVip(SeatPos pos);

    private:
        void checkBounds(SeatPos pos) const;

        int rows_;
        int cols_;
        std::vector<std::unique_ptr<Seat>> seats_;
    };

}