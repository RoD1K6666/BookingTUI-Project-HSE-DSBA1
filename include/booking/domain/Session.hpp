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
                std::shared_ptr<const Hall> hall);

        const SessionId& id() const noexcept;
        const std::string& title() const noexcept;

        const Hall& hall() const noexcept; // доступ к залу (без владения снаружи)

        bool isBooked(SeatPos pos) const;
        void book(SeatPos pos);
        void unbook(SeatPos pos);

        const std::unordered_set<SeatPos, SeatPosHash>& bookedSeats() const noexcept;

    private:
        SessionId id_;
        std::string title_;
        std::shared_ptr<const Hall> hall_;
        std::unordered_set<SeatPos, SeatPosHash> booked_;
    };

} // namespace booking::domain