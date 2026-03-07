#pragma once

#include <string>
#include <cstdint>

#include "booking/services/IdGenerator.hpp"

namespace booking::services {

    class SequentialIdGenerator final : public IdGenerator {
    public:
        [[nodiscard]] booking::domain::BookingId nextBookingId() override;

    private:
        std::uint64_t next_ = 1;
    };

}