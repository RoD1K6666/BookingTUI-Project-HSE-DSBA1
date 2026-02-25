#pragma once

#include <stdexcept>
#include <string>

#include "booking/domain/Types.hpp"

namespace booking::domain {

    class BookingError : public std::runtime_error {
    public:
        explicit BookingError(const std::string& message)
            : std::runtime_error(message) {}
    };

    class InvalidSeat : public BookingError {
    public:
        explicit InvalidSeat(SeatPos pos)
            : BookingError("Invalid seat position: row=" + std::to_string(pos.row) +
                           ", col=" + std::to_string(pos.col)),
              pos_(pos) {}

        [[nodiscard]] SeatPos pos() const noexcept { return pos_; }

    private:
        SeatPos pos_;
    };

    class SeatUnavailable : public BookingError {
    public:
        explicit SeatUnavailable(SeatPos pos)
            : BookingError("Seat is unavailable: row=" + std::to_string(pos.row) +
                           ", col=" + std::to_string(pos.col)),
              pos_(pos) {}

        [[nodiscard]] SeatPos pos() const noexcept { return pos_; }

    private:
        SeatPos pos_;
    };

    class SessionNotFound : public BookingError {
    public:
        explicit SessionNotFound(SessionId id)
            : BookingError("Session not found: " + id),
              id_(std::move(id)) {}

        [[nodiscard]] const SessionId& id() const noexcept { return id_; }

    private:
        SessionId id_;
    };

}