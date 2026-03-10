#include "booking/tui/SessionsScreen.hpp"
#include "booking/tui/App.hpp"
#include <iostream>

void SessionsScreen::render(App& app) {
    std::cout << "\n=== ДОСТУПНЫЕ СЕАНСЫ ===\n";
    for (const auto& s : app.getService().getSessions()) {
        std::cout << "ID: " << s.id << " | " << s.title << "\n";
    }
    std::cout << "0. Назад\n> Введите ID сеанса: ";

    int choice;
    if (!(std::cin >> choice)) { app.clearInput(); return; }
    if (choice == 0) { app.setState(AppState::MainMenu); return; }

    auto session_opt = app.getService().findSession(choice);
    if (session_opt.has_value()) {
        app.setSelectedSession(choice);
        app.setState(AppState::SeatMap);
    } else {
        std::cout << "Сеанс не найден!\n";
    }
}