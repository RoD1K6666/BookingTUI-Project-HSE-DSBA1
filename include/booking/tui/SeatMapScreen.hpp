#pragma once
#include "booking/tui/Screen.hpp"
#include "booking/domain/Session.hpp"

namespace booking::tui {

class App;

// ─────────────────────────────────────────────────────────────────────────────
//  SeatMapScreen
//
//  Renders the hall as a grid:
//      col:   1    2    3  ...
//   row A:  [ ]  [V]  [X]
//   row B:  [ ]  [ ]  [X]
//
//  [ ] = free regular   [V] = free VIP   [X] = occupied
//
//  User enters a seat (e.g. "A3") → BookingService::createBooking() is called.
//  Each booking is one seat; user is asked once per seat if they want another.
// ─────────────────────────────────────────────────────────────────────────────
class SeatMapScreen : public Screen {
public:
    // session is stored by reference inside App::sessions_ — stays alive
    SeatMapScreen(App& app, domain::Session& session);

    void        show() override;
    std::string name() const override { return "SeatMap"; }

private:
    App&             app_;
    domain::Session& session_;  // non-owning ref into App::sessions_

    // ── Rendering ─────────────────────────────────────────────────────────
    void renderMap() const;

    // ── Input ─────────────────────────────────────────────────────────────
    // Returns {-1,-1} on cancel / invalid
    domain::SeatPos askSeat() const;

    // ── Booking ───────────────────────────────────────────────────────────
    void doBook(domain::SeatPos pos);
};

} // namespace booking::tui
