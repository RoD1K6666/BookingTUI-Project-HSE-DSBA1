#include "booking/tui/SeatMapScreen.hpp"
#include "booking/tui/App.hpp"
#include <iostream>
#include <vector>
#include <type_traits>

void SeatMapScreen::render(App& app) {
    auto s_id = app.getSelectedSession();
    if (!s_id) { app.setState(AppState::Sessions); return; }

    auto session = app.getService().findSession(*s_id).value();
    std::vector<std::pair<int, int>> selected;
    int cur_r = 0, cur_c = 0;
    bool selecting = true;

    while (selecting) {
        std::cout << "\n=== ЗАЛ: " << session.title << " ===\n";
        for (int r = 0; r < session.rows; ++r) {
            for (int c = 0; c < session.cols; ++c) {
                const auto& seat = session.seat_grid[r][c];
                std::cout << (r == cur_r && c == cur_c ? "[" : " ");

                auto visitor = [&seat](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, Free>) return seat.is_vip ? "V" : ".";
                    else if constexpr (std::is_same_v<T, Held>) return "H";
                    else if constexpr (std::is_same_v<T, Booked>) return "X";
                };
                std::cout << std::visit(visitor, seat.state);

                std::cout << (r == cur_r && c == cur_c ? "]" : " ");
            }
            std::cout << "\n";
        }
        std::cout << "WASD + Enter (Движение) | x + Enter (Выбрать) | b + Enter (Бронь) | q + Enter (Назад)\n> ";
        char cmd; std::cin >> cmd;

        switch (cmd) {
            case 'w': if (cur_r > 0) cur_r--; break;
            case 's': if (cur_r < session.rows - 1) cur_r++; break;
            case 'a': if (cur_c > 0) cur_c--; break;
            case 'd': if (cur_c < session.cols - 1) cur_c++; break;
            case 'x':
                if (std::holds_alternative<Free>(session.seat_grid[cur_r][cur_c].state)) selected.push_back({cur_r, cur_c});
                break;
            case 'b':
                if (!selected.empty()) {
                    std::cout << "Введите имя: "; std::string name; std::cin >> name;
                    Booking b = app.getService().book(session.id, name, selected);
                    std::cout << "УСПЕШНО! Бронь ID: " << b.id << "\n";
                    selecting = false; app.setState(AppState::MainMenu);
                }
                break;
            case 'q': selecting = false; app.setState(AppState::Sessions); break;
        }
    }
}