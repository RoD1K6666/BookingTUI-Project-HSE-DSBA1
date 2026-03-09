#pragma once

#include <string>
#include <cstdint>

#include "booking/services/IdGenerator.hpp"

namespace booking::services {

    // Generates booking IDs in the form "booking-1", "booking-2", ...
    class SequentialIdGenerator final : public IdGenerator {
    public:
        [[nodiscard]] booking::domain::BookingId nextBookingId() override;

    private:
        std::uint64_t next_ = 1; // next number to use in the ID
    };

}