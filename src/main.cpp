//===============================================================================//
//          ЭТОТ МЕЙН ЧИСТО ДЛЯ ТЕСТА, МОЖНО МЕНЯТЬ И УДАЛЯТЬ КОГДА НАДО//
//===============================================================================//

#include <iostream>
#include <memory>

#include "booking/domain/Hall.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Errors.hpp"
#include "booking/services/BookingService.hpp"
#include "booking/services/SimplePricingService.hpp"
#include "booking/services/SequentialIdGenerator.hpp"

using namespace booking::domain;
using namespace booking::services;

int main() {
    try {
        // create a hall 3x4 with one VIP seat
        auto hall = std::make_shared<Hall>(3, 4);
        hall->makeVip({0, 0});

        // create a session
        Session session("s1", "Test movie", hall);

        // create services using real implementations
        SimplePricingService pricing(1000);
        SequentialIdGenerator ids;
        BookingService service(pricing, ids);

        // create two bookings
        Booking b1 = service.createBooking(session, {0, 0});
        std::cout << "  Created: " << b1.id()
                  << " price=" << b1.priceCents() << "\n";

        Booking b2 = service.createBooking(session, {1, 2});
        std::cout << "  Created: " << b2.id()
                  << " price=" << b2.priceCents() << "\n";

        // list all bookings
        std::cout << "\nAll bookings:\n";
        for (const auto& b : service.listBookings()) {
            std::cout << "  " << b.id()
                      << " price=" << b.priceCents() << "\n";
        }

        // find a booking
        auto found = service.findBooking("booking-1");
        std::cout << "\nFind booking-1:\n";
        if (found.has_value()) {
            std::cout << "  Found: " << found->id() << "\n";
        }

        auto notFound = service.findBooking("booking-999");
        std::cout << "Find booking-999:\n";
        if (!notFound.has_value()) {
            std::cout << "  Not found\n";
        }

        // сancel a booking
        service.cancelBooking("booking-1", session);
        std::cout << "\nAfter cancel:\n";
        for (const auto& b : service.listBookings()) {
            std::cout << "  " << b.id() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}