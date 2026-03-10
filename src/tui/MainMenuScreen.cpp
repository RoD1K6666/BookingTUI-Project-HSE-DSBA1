#include "booking/tui/MainMenuScreen.hpp"
#include "booking/tui/App.hpp"
#include <iostream>

void MainMenuScreen::render(App& app) {
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1. Список сеансов\n";
    std::cout << "2. Мои бронирования\n";
    std::cout << "0. Выход\n> ";

    int choice;
    if (!(std::cin >> choice)) { app.clearInput(); return; }

    switch (choice) {
        case 1: app.setState(AppState::Sessions); break;
        case 2: app.setState(AppState::Bookings); break;
        case 0: app.setState(AppState::Exit); break;
        default: std::cout << "Неверный выбор.\n";
    }
}