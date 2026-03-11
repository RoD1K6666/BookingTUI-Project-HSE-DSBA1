#include "booking/tui/MainMenuScreen.hpp"
#include "booking/tui/App.hpp"
#include "booking/tui/SessionsScreen.hpp"
#include "booking/tui/BookingsScreen.hpp"

#include <iostream>
#include <limits>

namespace booking::tui {

MainMenuScreen::MainMenuScreen(App& app) : app_(app) {}

void MainMenuScreen::show() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════╗\n";
    std::cout << "║       BookingTUI  v1.0       ║\n";
    std::cout << "╚══════════════════════════════╝\n";
    std::cout << "  [1] Browse sessions\n";
    std::cout << "  [2] My bookings\n";
    std::cout << "  [0] Exit\n\n";

    int choice = -1;
    while (true) {
        std::cout << "Your choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                app_.pushScreen(std::make_shared<SessionsScreen>(app_));
                return;
            case 2:
                app_.pushScreen(std::make_shared<BookingsScreen>(app_));
                return;
            case 0:
                app_.popScreen();
                return;
            default:
                std::cout << "  [!] Unknown option.\n";
        }
    }
}

} // namespace booking::tui
