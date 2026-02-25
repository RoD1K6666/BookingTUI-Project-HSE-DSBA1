
//===============================================================================//
//              ВРЕМЕННАЯ РЕАЛИЗАЦИЯ КОТОРАЯ НУЖНА ДЛЯ ПРОМЕЖУТОЧНОГО ТЕСТА//
//===============================================================================//






#pragma once

#include <cstdint>
#include "booking/domain/Types.hpp"   // SeatType

namespace booking::services {

    class PricingService {
    public:
        virtual ~PricingService() = default;

        virtual std::int64_t priceCents(booking::domain::SeatType type) const = 0;
    };

}