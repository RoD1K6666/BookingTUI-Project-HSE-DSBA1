#include "booking/services/SequentialIdGenerator.hpp"

namespace booking::services {

    booking::domain::BookingId SequentialIdGenerator::nextBookingId() {
        return "booking-" + std::to_string(next_++);
    }

}