//===============================================================================//
//          ЭТОТ МЕЙН ЧИСТО ДЛЯ ТЕСТА, МОЖНО МЕНЯТЬ И УДАЛЯТЬ КОГДА НАДО//
//===============================================================================//





#include <iostream>
#include <memory>

#include "booking/domain/Hall.hpp"
#include "booking/domain/Session.hpp"
#include "booking/services/BookingService.hpp"
#include "booking/services/PricingService.hpp"
#include "booking/services/IdGenerator.hpp"

using namespace booking::domain;
using namespace booking::services;

class DummyPricingService : public PricingService {
public:
    std::int64_t priceCents(SeatType type) const override {
        return (type == SeatType::Vip) ? 2000 : 1000;
    }
};

class DummyIdGenerator : public IdGenerator {
public:
    BookingId nextBookingId() override { return "booking-1"; }
};

int main() {
    try {
        auto hall = std::make_shared<Hall>(3, 4);
        hall->makeVip({0, 0});

        Session session("s1", "Test movie", hall);

        DummyPricingService pricing;
        DummyIdGenerator ids;
        BookingService service(pricing, ids);

        Booking b = service.createBooking(session, {0, 0});

        std::cout << "OK\n";
        std::cout << "booking_id=" << b.id() << "\n";
        std::cout << "session_id=" << b.sessionId() << "\n";
        std::cout << "price=" << b.priceCents() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}