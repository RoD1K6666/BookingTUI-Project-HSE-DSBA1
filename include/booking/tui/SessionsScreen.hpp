#pragma once
#include "booking/tui/Screen.hpp"
#include "booking/domain/Session.hpp"

namespace booking::tui {

class App;

// ─────────────────────────────────────────────
//  Shows the list of all sessions.
//  User picks one → SeatMapScreen.
// ─────────────────────────────────────────────
class SessionsScreen : public Screen {
public:
    explicit SessionsScreen(App& app);

    void        show() override;
    std::string name() const override { return "Sessions"; }

private:
    App& app_;

    // Count free seats: hall.rows()*cols() - session.bookedSeats().size()
    int freeSeats(const domain::Session& s) const;

    void printRow(std::size_t idx, const domain::Session& s) const;
};

} // namespace booking::tui
