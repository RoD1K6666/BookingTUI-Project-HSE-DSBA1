#pragma once

#include "booking/domain/Types.hpp"
#include "booking/services/PricingService.hpp"

namespace booking::services {
    constexpr double kVipMultiplier = 1.5; // VIP seat price multiplier

    class SimplePricingService final : public PricingService {
    public:
        explicit SimplePricingService(booking::domain::Money basePrice);

        [[nodiscard]] booking::domain::Money
        priceCents(booking::domain::SeatType type) const override;

    private:
        booking::domain::Money basePrice_;
    };

}