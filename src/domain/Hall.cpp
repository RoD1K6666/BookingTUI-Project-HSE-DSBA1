#include "booking/domain/Hall.hpp"

namespace booking::domain {

    Hall::Hall(int rows, int cols)
        : rows_(rows), cols_(cols)
    {
        if (rows <= 0 || cols <= 0) {
            throw BookingError("Hall dimensions must be positive");
        }

        seats_.reserve(rows * cols);

        for (int r = 0; r < rows_; ++r) {
            for (int c = 0; c < cols_; ++c) {
                seats_.push_back(
                    std::make_unique<RegularSeat>(SeatPos{r, c})
                );
            }
        }
    }

    int Hall::rows() const noexcept {
        return rows_;
    }

    int Hall::cols() const noexcept {
        return cols_;
    }

    void Hall::checkBounds(SeatPos pos) const {
        if (pos.row < 0 || pos.row >= rows_ ||
            pos.col < 0 || pos.col >= cols_) {
            throw InvalidSeat(pos);
            }
    }

    const Seat& Hall::seatAt(SeatPos pos) const {
        checkBounds(pos);
        return *seats_[pos.row * cols_ + pos.col];
    }

    Seat& Hall::seatAt(SeatPos pos) {
        checkBounds(pos);
        return *seats_[pos.row * cols_ + pos.col];
    }

    void Hall::makeVip(SeatPos pos) {
        checkBounds(pos);
        seats_[pos.row * cols_ + pos.col] =
            std::make_unique<VipSeat>(pos);
    }

}