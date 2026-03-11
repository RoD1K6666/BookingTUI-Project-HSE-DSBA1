#include "booking/tui/BookingsScreen.hpp"
#include "booking/tui/App.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>

namespace booking::tui {

BookingsScreen::BookingsScreen(App& app) : app_(app) {}

void BookingsScreen::show() {
    std::cout << "\n── My Bookings ──────────────────────\n";

    const auto& bookings = app_.service().listBookings();

    if (bookings.empty()) {
        std::cout << "  No bookings yet.\n";
        std::cout << "  Press Enter to go back...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        app_.popScreen();
        return;
    }

    for (std::size_t i = 0; i < bookings.size(); ++i)
        printRow(i + 1, bookings[i]);

    std::cout << "\n  Enter booking number to cancel, or 0 to go back:\n  > ";

    int choice = -1;
    while (true) {
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Enter a number: ";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            app_.popScreen();
            return;
        }
        if (choice >= 1 && choice <= static_cast<int>(bookings.size())) {
            tryCancel(bookings[choice - 1]);
            app_.popScreen();
            return;
        }
        std::cout << "  [!] Invalid. Try again: ";
    }
}

void BookingsScreen::printRow(std::size_t idx, const domain::Booking& b) const {
    std::cout << "  [" << idx << "] "
              << "ID: "      << b.id()
              << "  Session: " << b.sessionId()
              << "  Seat: "
              << static_cast<char>('A' + b.seatPos().row)
              << (b.seatPos().col + 1)
              << "  Type: "   << (b.seatType() == domain::SeatType::Vip ? "VIP" : "Regular")
              << "  Price: "  << b.priceCents() / 100.0 << " ₽"
              << "\n";
}

void BookingsScreen::tryCancel(const domain::Booking& booking) {
    std::cout << "  Cancel booking " << booking.id() << "? (y/n): ";
    char c;
    std::cin >> c;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (c != 'y' && c != 'Y') {
        std::cout << "  Cancelled.\n";
        return;
    }

    // cancelBooking() needs Session& to call session.unbook(pos)
    domain::Session* session = app_.findSession(booking.sessionId());
    if (!session) {
        std::cout << "  [!] Session not found — cannot cancel.\n";
        return;
    }

    try {
        app_.service().cancelBooking(booking.id(), *session);
        std::cout << "  ✔ Booking " << booking.id() << " cancelled.\n";
    } catch (const std::exception& e) {
        std::cout << "  [!] Error: " << e.what() << "\n";
    }
}

} // namespace booking::tui
