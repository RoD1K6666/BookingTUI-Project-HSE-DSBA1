#pragma once

#include "booking/domain/Types.hpp"

namespace booking::services {

    class IdGenerator {
    public:
        virtual ~IdGenerator() = default;

        [[nodiscard]] virtual booking::domain::BookingId nextBookingId() = 0;
    };

}