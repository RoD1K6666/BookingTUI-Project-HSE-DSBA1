#pragma once

#include "booking/domain/Types.hpp"

namespace booking::services {

    class PricingService {
    public:
        virtual ~PricingService() = default;

        [[nodiscard]] virtual booking::domain::Money
        priceCents(booking::domain::SeatType type) const = 0;
    };

}