#pragma once
#include "booking/tui/Screen.hpp"

namespace booking::tui {

class App;

// Options: [1] Sessions  [2] My bookings  [0] Exit
class MainMenuScreen : public Screen {
public:
    explicit MainMenuScreen(App& app);

    void        show() override;
    std::string name() const override { return "MainMenu"; }

private:
    App& app_;
};

} // namespace booking::tui
