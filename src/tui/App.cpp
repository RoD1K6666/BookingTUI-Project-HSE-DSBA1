#include "booking/tui/App.hpp"
#include "booking/tui/MainMenuScreen.hpp"
#include "booking/tui/SessionsScreen.hpp"
#include "booking/tui/SeatMapScreen.hpp"
#include "booking/tui/BookingsScreen.hpp"
#include <iostream>
#include <limits>

App::App(BookingService& srv) : service(srv), current_state(AppState::MainMenu) {}

void App::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void App::run() {
    MainMenuScreen mainMenu;
    SessionsScreen sessionsMenu;
    SeatMapScreen seatMapMenu;
    BookingsScreen bookingsMenu;

    while (current_state != AppState::Exit) {
        try {
            switch (current_state) {
                case AppState::MainMenu: mainMenu.render(*this); break;
                case AppState::Sessions: sessionsMenu.render(*this); break;
                case AppState::SeatMap:  seatMapMenu.render(*this); break;
                case AppState::Bookings: bookingsMenu.render(*this); break;
                case AppState::Exit: break;
            }
        } catch (const BookingError& e) {
            std::cout << "\n[ОШИБКА БИЗНЕС-ЛОГИКИ]: " << e.what() << "\nНажмите Enter...";
            clearInput(); std::cin.get();
        } catch (const std::exception& e) {
            std::cout << "\n[СИСТЕМНАЯ ОШИБКА]: " << e.what() << "\n";
            current_state = AppState::MainMenu;
        }
    }
    std::cout << "Завершение работы.\n";
}