#include "booking/services/BookingService.hpp"

#include <utility>

#include "booking/domain/Errors.hpp"
#include "booking/services/IdGenerator.hpp"
#include "booking/services/PricingService.hpp"

namespace booking::services {

    BookingService::BookingService(PricingService& pricing, IdGenerator& ids)
        : pricing_(pricing), ids_(ids) {}

    booking::domain::Booking
    BookingService::createBooking(booking::domain::Session& session, booking::domain::SeatPos pos) {
        // Look up the seat and determine its type
        const booking::domain::Seat& seat = session.hall().seatAt(pos);
        const booking::domain::SeatType type = seat.type();

        // Calculate price based on seat type
        const booking::domain::Money price = pricing_.priceCents(type);

        // Mark the seat as booked in the session
        session.book(pos);

        // Generate a unique ID and store the booking
        booking::domain::BookingId bookingId = ids_.nextBookingId();
        bookings_.emplace_back(bookingId, session.id(), pos, type, price);
        return bookings_.back();
    }

    const std::vector<booking::domain::Booking>& BookingService::listBookings() const {
        return bookings_;
    }

    std::optional<booking::domain::Booking>
    BookingService::findBooking(const booking::domain::BookingId& id) const {
        for (const auto& b : bookings_) {
            if (b.id() == id) {
                return b; // found - return the booking
            }
        }
        return std::nullopt; // not found - return empty
    }

    void BookingService::cancelBooking(const booking::domain::BookingId& id,
                                       booking::domain::Session& session) {
        // Search for the booking by ID
        auto it = std::find_if(bookings_.begin(), bookings_.end(),
            [&id](const booking::domain::Booking& b) {
                return b.id() == id;
            });

        if (it == bookings_.end()) {
            throw booking::domain::BookingError("Booking not found: " + id);
        }

        // Free the seat and remove the booking
        session.unbook(it->seatPos());
        bookings_.erase(it);
    }

    // Variant-based booking: returns Booking on success or BookingFailure on error
    BookingResult BookingService::tryBook(booking::domain::Session& session,
                                          booking::domain::SeatPos pos) {
        try {
            return createBooking(session, pos);
        } catch (const booking::domain::BookingError& e) {
            return BookingFailure{e.what()};
        }
    }

}