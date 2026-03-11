#pragma once

#include <string>
#include <utility>

#include "booking/domain/Types.hpp"

namespace booking::domain {

    class Booking {
    public:
        Booking(BookingId id,
                SessionId sessionId,
                SeatPos seatPos,
                SeatType seatType,
                Money priceCents)
            : id_(std::move(id)),
              sessionId_(std::move(sessionId)),
              seatPos_(seatPos),
              seatType_(seatType),
              priceCents_(priceCents) {}

        [[nodiscard]] const BookingId& id() const noexcept { return id_; }
        [[nodiscard]] const SessionId& sessionId() const noexcept { return sessionId_; }
        [[nodiscard]] SeatPos seatPos() const noexcept { return seatPos_; }
        [[nodiscard]] SeatType seatType() const noexcept { return seatType_; }
        [[nodiscard]] Money priceCents() const noexcept { return priceCents_; }

    private:
        BookingId id_;
        SessionId sessionId_;
        SeatPos seatPos_;
        SeatType seatType_;
        Money priceCents_;
    };

}