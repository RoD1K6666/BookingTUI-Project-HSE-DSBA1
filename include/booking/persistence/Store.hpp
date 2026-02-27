#pragma once

#include <vector>
#include <unordered_map>

#include "booking/domain/Session.hpp"
#include "booking/domain/Booking.hpp"

namespace booking::persistence {

    struct Store {
        std::vector<booking::domain::Session> sessions;
        std::vector<booking::domain::Booking> bookings;
        std::unordered_map<int, std::vector<int>> occupancy;
    };

}