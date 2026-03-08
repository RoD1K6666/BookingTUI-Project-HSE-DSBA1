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
        const booking::domain::Seat& seat = session.hall().seatAt(pos);
        const booking::domain::SeatType type = seat.type();

        const booking::domain::Money price = pricing_.priceCents(type);

        session.book(pos);

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
                return b; // нашли - возвращаем бронь
            }
        }
        return std::nullopt; // не нашли — возвращаем "пусто"
    }

    void BookingService::cancelBooking(const booking::domain::BookingId& id,
                                       booking::domain::Session& session) {
        auto it = std::find_if(bookings_.begin(), bookings_.end(),
            [&id](const booking::domain::Booking& b) {
                return b.id() == id;
            });

        if (it == bookings_.end()) {
            throw booking::domain::BookingError("Booking not found: " + id);
        }

        session.unbook(it->seatPos());
        bookings_.erase(it);
    }

}