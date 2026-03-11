#pragma once
#include <string>

namespace booking::tui {

// Base interface for all TUI screens
class Screen {
public:
    virtual ~Screen() = default;

    // Called each time the screen becomes active
    virtual void show() = 0;

    virtual std::string name() const = 0;
};

} // namespace booking::tui
