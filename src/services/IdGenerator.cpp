#include "booking/services/SequentialIdGenerator.hpp"

namespace booking::services {

    // Returns "booking-N" and increments the counter
    booking::domain::BookingId SequentialIdGenerator::nextBookingId() {
        return "booking-" + std::to_string(next_++);
    }

}