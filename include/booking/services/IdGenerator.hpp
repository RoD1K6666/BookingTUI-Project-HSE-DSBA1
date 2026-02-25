
//===============================================================================//
//              ВРЕМЕННАЯ РЕАЛИЗАЦИЯ КОТОРАЯ НУЖНА ДЛЯ ПРОМЕЖУТОЧНОГО ТЕСТА//
//===============================================================================//




#pragma once

#include <string>
#include "booking/domain/Types.hpp"   // BookingId

namespace booking::services {

    class IdGenerator {
    public:
        virtual ~IdGenerator() = default;

        virtual booking::domain::BookingId nextBookingId() = 0;
    };

} // namespace booking::services