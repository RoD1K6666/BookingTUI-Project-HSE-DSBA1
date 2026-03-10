#include "booking/tui/BookingsScreen.hpp"
#include "booking/tui/App.hpp"
#include <iostream>

void BookingsScreen::render(App& app) {
    std::cout << "\n=== МОИ БРОНИ ===\nВведите номер (0 - назад): ";
    int b_id;
    if (!(std::cin >> b_id) || b_id == 0) {
        app.clearInput(); app.setState(AppState::MainMenu); return;
    }

    auto b_opt = app.getService().findBooking(b_id);
    if (b_opt.has_value()) {
        std::cout << "Имя: " << b_opt->name << " | Сумма: " << b_opt->sum << "\nОтменить? (y/n): ";
        char ans; std::cin >> ans;
        if (ans == 'y') {
            app.getService().cancel(b_opt->id);
            std::cout << "Успешно отменено.\n";
        }
    } else {
        std::cout << "Бронь не найдена.\n";
    }
}