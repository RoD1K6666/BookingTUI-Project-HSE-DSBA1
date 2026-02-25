#pragma once

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

    private:
        PricingService& pricing_;
        IdGenerator& ids_;
    };

}