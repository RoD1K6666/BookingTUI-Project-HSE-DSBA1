#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "booking/domain/Types.hpp"
#include "booking/domain/Hall.hpp"

namespace booking::domain {

    class Session {
    public:
        Session(SessionId id,
                std::string title,
                HallId hallId,
                std::shared_ptr<const Hall> hall);

        const HallId& hallId() const noexcept;
        const SessionId& id() const noexcept;
        const std::string& title() const noexcept;

        const Hall& hall() const noexcept;

        bool isBooked(SeatPos pos) const;
        void book(SeatPos pos);
        void unbook(SeatPos pos);

        const std::unordered_set<SeatPos, SeatPosHash>& bookedSeats() const noexcept;

    private:
        SessionId id_;
        std::string title_;
        HallId hallId_;
        std::shared_ptr<const Hall> hall_;
        std::unordered_set<SeatPos, SeatPosHash> booked_;
    };

}