#include "booking/tui/SeatMapScreen.hpp"
#include "booking/tui/App.hpp"
#include "booking/domain/Hall.hpp"
#include "booking/domain/Types.hpp"

#include <iostream>
#include <limits>
#include <cctype>
#include <stdexcept>

namespace booking::tui {

SeatMapScreen::SeatMapScreen(App& app, domain::Session& session)
    : app_(app), session_(session) {}

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


} // namespace booking::tui
