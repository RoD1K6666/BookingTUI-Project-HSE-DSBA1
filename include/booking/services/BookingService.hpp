#pragma once

#include <vector>
#include <optional>

#include "booking/domain/Types.hpp"
#include "booking/domain/Booking.hpp"
#include "booking/domain/Session.hpp"

namespace booking::services {

    class PricingService;
    class IdGenerator;

    class BookingService {
    public:
        BookingService(PricingService& pricing, IdGenerator& ids);

        [[nodiscard]] booking::domain::Booking createBooking(booking::domain::Session& session,
                                                             booking::domain::SeatPos pos);

        [[nodiscard]] const std::vector<booking::domain::Booking>& listBookings() const;

        [[nodiscard]] std::optional<booking::domain::Booking>
        findBooking(const booking::domain::BookingId& id) const;

    private:
        PricingService& pricing_;
        IdGenerator& ids_;
        std::vector<booking::domain::Booking> bookings_;
    };

}