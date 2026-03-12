#include <gtest/gtest.h>
#include <memory>

#include "booking/domain/Hall.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Errors.hpp"
#include "booking/domain/RegularSeat.hpp"

using namespace booking::domain;

// Hall tests

TEST(HallTest, ConstructorCreatesCorrectSize) {
    Hall hall(3, 4);
    EXPECT_EQ(hall.rows(), 3);
    EXPECT_EQ(hall.cols(), 4);
}

TEST(HallTest, AllSeatsAreRegularByDefault) {
    Hall hall(2, 2);
    EXPECT_EQ(hall.seatAt({0, 0}).type(), SeatType::Regular);
    EXPECT_EQ(hall.seatAt({1, 1}).type(), SeatType::Regular);
}

TEST(HallTest, MakeVipChangesSeatType) {
    Hall hall(2, 2);
    hall.makeVip({0, 0});
    EXPECT_EQ(hall.seatAt({0, 0}).type(), SeatType::Vip);
    EXPECT_EQ(hall.seatAt({0, 1}).type(), SeatType::Regular);
}

TEST(HallTest, SeatAtReturnsCorrectPosition) {
    Hall hall(3, 4);
    const Seat& seat = hall.seatAt({2, 3});
    EXPECT_EQ(seat.position().row, 2);
    EXPECT_EQ(seat.position().col, 3);
}

TEST(HallTest, InvalidDimensionsThrows) {
    EXPECT_THROW(Hall(0, 5), BookingError);
    EXPECT_THROW(Hall(5, -1), BookingError);
}

TEST(HallTest, OutOfBoundsSeatThrows) {
    Hall hall(3, 4);
    EXPECT_THROW(hall.seatAt({5, 0}), InvalidSeat);
    EXPECT_THROW(hall.seatAt({0, 10}), InvalidSeat);
    EXPECT_THROW(hall.seatAt({-1, 0}), InvalidSeat);
}

// Session tests

TEST(SessionTest, ConstructorStoresData) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);
    EXPECT_EQ(s.id(), "s1");
    EXPECT_EQ(s.title(), "Movie");
    EXPECT_EQ(s.hallId(), "h1");
}

TEST(SessionTest, HallAccessor) {
    auto hall = std::make_shared<Hall>(3, 4);
    Session s("s1", "Movie", "h1", hall);
    EXPECT_EQ(s.hall().rows(), 3);
    EXPECT_EQ(s.hall().cols(), 4);
}

TEST(SessionTest, NullHallThrows) {
    EXPECT_THROW(Session("s1", "Movie", "h1", nullptr), BookingError);
}

TEST(SessionTest, BookAndIsBooked) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);

    EXPECT_FALSE(s.isBooked({0, 0}));
    s.book({0, 0});
    EXPECT_TRUE(s.isBooked({0, 0}));
    EXPECT_FALSE(s.isBooked({0, 1}));
}

TEST(SessionTest, BookSameSeatTwiceThrows) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);

    s.book({0, 0});
    EXPECT_THROW(s.book({0, 0}), SeatUnavailable);
}

TEST(SessionTest, UnbookFreesSeat) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);

    s.book({0, 0});
    s.unbook({0, 0});
    EXPECT_FALSE(s.isBooked({0, 0}));
}

TEST(SessionTest, BookedSeatsReturnsSet) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);

    s.book({0, 0});
    s.book({1, 1});
    EXPECT_EQ(s.bookedSeats().size(), 2);
}

TEST(SessionTest, BookOutOfBoundsThrows) {
    auto hall = std::make_shared<Hall>(2, 2);
    Session s("s1", "Movie", "h1", hall);

    EXPECT_THROW(s.book({5, 5}), InvalidSeat);
}
