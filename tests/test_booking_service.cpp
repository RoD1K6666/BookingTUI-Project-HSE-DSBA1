#include <gtest/gtest.h>
#include <memory>

#include "booking/domain/Hall.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Errors.hpp"
#include "booking/services/BookingService.hpp"
#include "booking/services/SimplePricingService.hpp"
#include "booking/services/SequentialIdGenerator.hpp"

using namespace booking::domain;
using namespace booking::services;

// Helper: creates a hall and session for testing
static std::pair<std::shared_ptr<Hall>, Session> makeTestSession() {
    auto hall = std::make_shared<Hall>(3, 4);
    hall->makeVip({0, 0});
    Session session("s1", "Test Movie", hall);
    return {hall, std::move(session)};
}

// Booking creation and search tests

TEST(BookingServiceTest, CreateBookingSavesAndReturnsBooking) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    Booking b = service.createBooking(session, {1, 0});

    EXPECT_EQ(b.id(), "booking-1");
    EXPECT_EQ(b.priceCents(), 1000);
    EXPECT_EQ(b.seatType(), SeatType::Regular);
    EXPECT_EQ(service.listBookings().size(), 1);
}

TEST(BookingServiceTest, CreateBookingVipSeatHasHigherPrice) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    Booking b = service.createBooking(session, {0, 0});

    EXPECT_EQ(b.priceCents(), 1500);
    EXPECT_EQ(b.seatType(), SeatType::Vip);
}

TEST(BookingServiceTest, FindBookingReturnsCorrectBooking) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    service.createBooking(session, {1, 0});

    auto found = service.findBooking("booking-1");
    EXPECT_TRUE(found.has_value());
    EXPECT_EQ(found->id(), "booking-1");

    auto notFound = service.findBooking("booking-999");
    EXPECT_FALSE(notFound.has_value());
}

// Booking cancellation and error handling tests

TEST(BookingServiceTest, CancelBookingRemovesIt) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    service.createBooking(session, {1, 0});
    EXPECT_EQ(service.listBookings().size(), 1);

    service.cancelBooking("booking-1", session);
    EXPECT_EQ(service.listBookings().size(), 0);
    EXPECT_FALSE(session.isBooked({1, 0}));
}

TEST(BookingServiceTest, CancelNonExistentBookingThrows) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    EXPECT_THROW(service.cancelBooking("booking-999", session), BookingError);
}

TEST(BookingServiceTest, BookingAlreadyTakenSeatThrows) {
    auto [hall, session] = makeTestSession();
    SimplePricingService pricing(1000);
    SequentialIdGenerator ids;
    BookingService service(pricing, ids);

    service.createBooking(session, {1, 0});

    EXPECT_THROW(service.createBooking(session, {1, 0}), SeatUnavailable);
}