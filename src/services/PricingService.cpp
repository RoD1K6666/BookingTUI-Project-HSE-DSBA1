#include "booking/services/SimplePricingService.hpp"

namespace booking::services {

    SimplePricingService::SimplePricingService(booking::domain::Money basePrice)
        : basePrice_(basePrice) {}

    booking::domain::Money
    SimplePricingService::priceCents(booking::domain::SeatType type) const {
        if (type == booking::domain::SeatType::Vip) {
            return static_cast<booking::domain::Money>(
                static_cast<double>(basePrice_) * kVipMultiplier
            );
        }
        return basePrice_;
    }

}
