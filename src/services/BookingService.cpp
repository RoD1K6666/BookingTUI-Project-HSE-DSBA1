#include "booking/services/BookingService.hpp"
#include "booking/services/PricingService.hpp"
#include "booking/services/IdGenerator.hpp"
#include "booking/domain/Errors.hpp"
namespace booking::services {

    BookingService::BookingService(PricingService& pricing, IdGenerator& ids)
        : pricing_(pricing), ids_(ids) {}

    booking::domain::Booking BookingService::createBooking(booking::domain::Session& session,
                                                          booking::domain::SeatPos pos) {
        using namespace booking::domain;

        // 1) проверить место существует (Session внутри тоже проверяет)
        const Seat& seat = session.hall().seatAt(pos);
        const SeatType type = seat.type();

        // 2) посчитать цену
        // !!! ВАЖНО: подстрой под свой PricingService
        // Я предполагаю интерфейс: std::int64_t priceCents(SeatType)
        const std::int64_t price = pricing_.priceCents(type);

        // 3) попытаться занять место (может бросить InvalidSeat / SeatUnavailable)
        session.book(pos);

        // 4) создать бронь
        BookingId bookingId = ids_.nextBookingId(); // подстрой под свой IdGenerator
        return {std::move(bookingId), session.id(), pos, type, price};
    }

} // namespace booking::services