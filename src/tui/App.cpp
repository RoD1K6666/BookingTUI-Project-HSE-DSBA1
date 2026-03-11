#include "booking/tui/App.hpp"
#include "booking/tui/Screen.hpp"
#include "booking/tui/MainMenuScreen.hpp"

namespace booking::tui {

App::App(services::BookingService&    service,
         std::vector<domain::Session> sessions)
    : service_(service)
    , sessions_(std::move(sessions))
{
    pushScreen(std::make_shared<MainMenuScreen>(*this));
}

void App::run() {
    while (!screenStack_.empty())
        screenStack_.top()->show();
}

void App::pushScreen(std::shared_ptr<Screen> screen) {
    screenStack_.push(std::move(screen));
}

void App::popScreen() {
    if (!screenStack_.empty())
        screenStack_.pop();
}

services::BookingService& App::service() noexcept {
    return service_;
}

std::vector<domain::Session>& App::sessions() noexcept {
    return sessions_;
}

domain::Session* App::findSession(const domain::SessionId& id) {
    for (auto& s : sessions_)
        if (s.id() == id) return &s;
    return nullptr;
}

} // namespace booking::tui
