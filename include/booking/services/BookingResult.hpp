#pragma once

#include <variant>
#include <string>

#include "booking/domain/Booking.hpp"

namespace booking::services {

    // Error description when booking fails
    struct BookingFailure {
        std::string reason;
    };

    // Result of a booking attempt: either a Booking or a failure reason
    using BookingResult = std::variant<booking::domain::Booking, BookingFailure>;

    // Visit helper to check if the result is successful
    inline bool isSuccess(const BookingResult& result) {
        return std::holds_alternative<booking::domain::Booking>(result);
    }

}
