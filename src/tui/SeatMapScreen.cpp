#include "booking/tui/SeatMapScreen.hpp"
#include "booking/tui/App.hpp"
#include "booking/domain/Hall.hpp"
#include "booking/domain/Types.hpp"

#include <iostream>
#include <limits>
#include <cctype>
#include <stdexcept>
#include <variant>
#include "booking/services/BookingResult.hpp"

namespace booking::tui {

SeatMapScreen::SeatMapScreen(App& app, domain::Session& session)
    : app_(app), session_(session) {}

// ── Main flow ────────────────────────────────────────────────────────────────
void SeatMapScreen::show() {
    while (true) {
        std::cout << "\n── " << session_.title() << " ────\n";
        renderMap();
        std::cout << "\n  Legend:  [ ] regular   [V] VIP   [X] occupied\n";
        std::cout << "  Enter seat (e.g. A3) or 0 to go back: ";

        domain::SeatPos pos = askSeat();
        if (pos.row == -1) {           // user typed 0 or invalid → go back
            app_.popScreen();
            return;
        }

        if (session_.isBooked(pos)) {
            std::cout << "  [!] That seat is already taken.\n";
            continue;
        }

        doBook(pos);

        // Ask if they want to book another seat in the same session
        std::cout << "  Book another seat in this session? (y/n): ";
        char again;
        std::cin >> again;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (again != 'y' && again != 'Y') {
            app_.popScreen();
            return;
        }
    }
}

// ── Render ───────────────────────────────────────────────────────────────────
void SeatMapScreen::renderMap() const {
    const auto& hall = session_.hall();
    int rows = hall.rows();
    int cols = hall.cols();

    // Column header
    std::cout << "\n       ";
    for (int c = 1; c <= cols; ++c)
        std::cout << "  " << c << " ";
    std::cout << "\n";
    std::cout << "        ═══ SCREEN ═══\n\n";

    for (int r = 0; r < rows; ++r) {
        // Row label: A, B, C …
        std::cout << "  " << static_cast<char>('A' + r) << "   ";

        for (int c = 0; c < cols; ++c) {
            domain::SeatPos pos{r, c};
            char sym;
            if (session_.isBooked(pos)) {
                sym = 'X';
            } else if (hall.seatAt(pos).type() == domain::SeatType::Vip) {
                sym = 'V';
            } else {
                sym = ' ';
            }
            std::cout << "[" << sym << "]";
            if (c + 1 < cols) std::cout << " ";
        }
        std::cout << "\n";
    }
}

// ── Input: parse "A3" → SeatPos{0, 2} ────────────────────────────────────────
domain::SeatPos SeatMapScreen::askSeat() const {
    std::string input;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (input == "0") return {-1, -1};

    if (input.size() < 2) return {-1, -1};

    char rowChar = static_cast<char>(std::toupper(static_cast<unsigned char>(input[0])));
    if (rowChar < 'A') return {-1, -1};
    int row = rowChar - 'A';

    int col = -1;
    try {
        col = std::stoi(input.substr(1)) - 1; // user enters 1-based column
    } catch (...) {
        return {-1, -1};
    }

    const auto& hall = session_.hall();
    if (row < 0 || row >= hall.rows() || col < 0 || col >= hall.cols()) {
        std::cout << "  [!] Seat " << input << " is out of range.\n";
        return {-1, -1};
    }

    return {row, col};
}

// ── Book one seat (uses std::variant + std::visit instead of exceptions) ─────
void SeatMapScreen::doBook(domain::SeatPos pos) {
    services::BookingResult result = app_.service().tryBook(session_, pos);

    std::visit([](auto& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, domain::Booking>) {
            std::cout << "\n  ✔ Booking confirmed!\n";
            std::cout << "    Booking ID : " << val.id()         << "\n";
            std::cout << "    Session    : " << val.sessionId()  << "\n";
            std::cout << "    Seat       : "
                      << static_cast<char>('A' + val.seatPos().row)
                      << (val.seatPos().col + 1)                 << "\n";
            std::cout << "    Type       : "
                      << (val.seatType() == domain::SeatType::Vip ? "VIP" : "Regular") << "\n";
            std::cout << "    Price      : " << val.priceCents() / 100.0 << " ₽\n";
        } else {
            std::cout << "  [!] Booking failed: " << val.reason << "\n";
        }
    }, result);
}

}