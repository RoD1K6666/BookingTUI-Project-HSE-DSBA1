#include "booking/tui/SessionsScreen.hpp"
#include "booking/tui/App.hpp"
#include "booking/tui/SeatMapScreen.hpp"

#include <iostream>
#include <limits>

namespace booking::tui {

SessionsScreen::SessionsScreen(App& app) : app_(app) {}

int SessionsScreen::freeSeats(const domain::Session& s) const {
    const auto& hall = s.hall();
    int total = hall.rows() * hall.cols();
    return total - static_cast<int>(s.bookedSeats().size());
}

void SessionsScreen::printRow(std::size_t idx, const domain::Session& s) const {
    const auto& hall = s.hall();
    std::cout << "  [" << idx << "] "
              << s.title()
              << "  |  Hall: " << hall.rows() << "×" << hall.cols()
              << "  |  Free seats: " << freeSeats(s)
              << "\n";
}

void SessionsScreen::show() {
    std::cout << "\n── Sessions ─────────────────────────\n";

    auto& sessions = app_.sessions();

    if (sessions.empty()) {
        std::cout << "  No sessions available.\n";
        std::cout << "  Press Enter to go back...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        app_.popScreen();
        return;
    }

    for (std::size_t i = 0; i < sessions.size(); ++i)
        printRow(i + 1, sessions[i]);

    std::cout << "  [0] Back\n\n";

    int choice = -1;
    while (true) {
        std::cout << "Select session: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            app_.popScreen();
            return;
        }
        if (choice >= 1 && choice <= static_cast<int>(sessions.size())) {
            // Pass reference into App::sessions_ — it stays alive as long as App does
            app_.pushScreen(std::make_shared<SeatMapScreen>(app_, sessions[choice - 1]));
            return;
        }
        std::cout << "  [!] Invalid choice.\n";
    }
}

} // namespace booking::tui
