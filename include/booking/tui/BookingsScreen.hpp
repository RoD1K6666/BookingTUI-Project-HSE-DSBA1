#pragma once
#include "booking/tui/Screen.hpp"
#include "booking/domain/Booking.hpp"

namespace booking::tui {

class App;

// ─────────────────────────────────────────────────────────────────────────────
//  BookingsScreen
//  Lists all bookings. User can select one to cancel.
//  cancelBooking() requires both BookingId AND Session& → we look up the
//  session from App::findSession().
// ─────────────────────────────────────────────────────────────────────────────
class BookingsScreen : public Screen {
public:
    explicit BookingsScreen(App& app);

    void        show() override;
    std::string name() const override { return "Bookings"; }

private:
    App& app_;

    void printRow(std::size_t idx, const domain::Booking& b) const;
    void tryCancel(const domain::Booking& booking);
};

} // namespace booking::tui
