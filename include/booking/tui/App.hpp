#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "booking/services/BookingService.hpp"
#include "booking/domain/Session.hpp"

namespace booking::tui {

class Screen;

// ─────────────────────────────────────────────────────────────────────────────
//  App — owns the screen stack, holds a reference to BookingService and
//        the full list of sessions (loaded at startup from persistence/mock).
//
//  Navigation:
//    pushScreen(screen)  — open a new screen on top
//    popScreen()         — return to the previous screen
//    run()               — main event loop (runs until stack is empty)
// ─────────────────────────────────────────────────────────────────────────────
class App {
public:
    App(services::BookingService&    service,
        std::vector<domain::Session> sessions);

    // Main loop — calls show() on the top screen until the stack is empty
    void run();

    // ── Navigation ────────────────────────────────────────────────────────
    void pushScreen(std::shared_ptr<Screen> screen);
    void popScreen();

    // ── Data accessors ────────────────────────────────────────────────────
    services::BookingService& service() noexcept;

    // All sessions known to the app (mutable so screens can call session.book/unbook)
    std::vector<domain::Session>& sessions() noexcept;

    // Find session by id; returns nullptr if not found
    domain::Session* findSession(const domain::SessionId& id);

private:
    services::BookingService&           service_;
    std::vector<domain::Session>        sessions_;
    std::stack<std::shared_ptr<Screen>> screenStack_;
};

} // namespace booking::tui
