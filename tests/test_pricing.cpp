#include <gtest/gtest.h>

#include "booking/services/SimplePricingService.hpp"
#include "booking/services/SequentialIdGenerator.hpp"

using namespace booking::domain;
using namespace booking::services;

// SimplePricingService tests

TEST(PricingServiceTest, RegularSeatReturnsBasePrice) {
    SimplePricingService pricing(1000);
    EXPECT_EQ(pricing.priceCents(SeatType::Regular), 1000);
}

TEST(PricingServiceTest, VipSeatReturnsMultipliedPrice) {
    SimplePricingService pricing(1000);
    EXPECT_EQ(pricing.priceCents(SeatType::Vip), 1500);
}

TEST(PricingServiceTest, DifferentBasePrice) {
    SimplePricingService pricing(2000);
    EXPECT_EQ(pricing.priceCents(SeatType::Regular), 2000);
    EXPECT_EQ(pricing.priceCents(SeatType::Vip), 3000);
}

// SequentialIdGenerator tests

TEST(IdGeneratorTest, FirstIdIsBooking1) {
    SequentialIdGenerator ids;
    EXPECT_EQ(ids.nextBookingId(), "booking-1");
}

TEST(IdGeneratorTest, IdsAreSequential) {
    SequentialIdGenerator ids;
    EXPECT_EQ(ids.nextBookingId(), "booking-1");
    EXPECT_EQ(ids.nextBookingId(), "booking-2");
    EXPECT_EQ(ids.nextBookingId(), "booking-3");
}

TEST(IdGeneratorTest, TwoGeneratorsAreIndependent) {
    SequentialIdGenerator ids1;
    SequentialIdGenerator ids2;
    EXPECT_EQ(ids1.nextBookingId(), "booking-1");
    EXPECT_EQ(ids2.nextBookingId(), "booking-1");
}