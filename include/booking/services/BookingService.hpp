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

        // Creates a booking, marks the seat as taken, and stores it
        [[nodiscard]] booking::domain::Booking createBooking(booking::domain::Session& session,
                                                             booking::domain::SeatPos pos);

        // Returns all stored bookings
        [[nodiscard]] const std::vector<booking::domain::Booking>& listBookings() const;

        // Finds a booking by ID, returns std::nullopt if not found
        [[nodiscard]] std::optional<booking::domain::Booking>
        findBooking(const booking::domain::BookingId& id) const;

        // Cancels a booking: frees the seat and removes it from storage
        void cancelBooking(const booking::domain::BookingId& id,
                           booking::domain::Session& session);

    private:
        PricingService& pricing_;
        IdGenerator& ids_;
        std::vector<booking::domain::Booking> bookings_;
    };

}