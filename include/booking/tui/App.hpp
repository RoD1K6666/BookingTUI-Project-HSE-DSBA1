#pragma once
#include "#include/booking/services/BookingService.hpp"
#include <optional>

enum class AppState { MainMenu, Sessions, SeatMap, Bookings, Exit };

class App {
public:
    explicit App(BookingService& srv);
    void run();

    BookingService& getService() { return service; }
    void setState(AppState state) { current_state = state; }

    void setSelectedSession(int id) { selected_session_id = id; }
    std::optional<int> getSelectedSession() const { return selected_session_id; }

    void clearInput();

private:
    BookingService& service;
    AppState current_state;
    std::optional<int> selected_session_id;
};