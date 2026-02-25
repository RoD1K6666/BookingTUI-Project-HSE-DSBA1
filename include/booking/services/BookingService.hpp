#pragma once

#include "booking/domain/Booking.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Types.hpp"
#include "booking/services/PricingService.hpp"
#include "booking/services/IdGenerator.hpp"

namespace booking::services {

    class BookingService {
    public:
        BookingService(PricingService& pricing, IdGenerator& ids);

        booking::domain::Booking createBooking(booking::domain::Session& session,
                                              booking::domain::SeatPos pos);

    private:
        PricingService& pricing_;
        IdGenerator& ids_;
    };

}