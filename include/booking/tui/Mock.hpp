#pragma once
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <stdexcept>

struct Free {}; struct Held { std::string until; }; struct Booked { int id; };
using SeatState = std::variant<Free, Held, Booked>;

struct SeatInfo { bool is_vip; SeatState state; };
struct Session { int id; std::string title; int rows=5; int cols=5; std::vector<std::vector<SeatInfo>> seat_grid; };
struct Booking { int id; std::string name; double sum; };

class BookingError : public std::runtime_error { public: BookingError(const std::string& msg) : std::runtime_error(msg) {} };

class BookingService {
public:
    std::vector<Session> getSessions() {
        Session s1{1, "Dune 2"};
        s1.seat_grid.resize(s1.rows, std::vector<SeatInfo>(s1.cols, {false, Free{}}));
        s1.seat_grid[2][2] = {true, Free{}}; s1.seat_grid[0][0].state = Booked{101};
        return {s1};
    }
    std::optional<Session> findSession(int id) {
        if (id == 1) return getSessions()[0];
        return std::nullopt;
    }
    std::optional<Booking> findBooking(int id) {
        if (id == 101) return Booking{101, "Ivan", 1500};
        return std::nullopt;
    }
    Booking book(int, const std::string& name, const std::vector<std::pair<int, int>>& seats) {
        if (seats.empty()) throw BookingError("Места не выбраны!");
        return Booking{102, name, 500.0 * seats.size()};
    }
    void cancel(int id) { if (id != 101) throw BookingError("Бронь не найдена!"); }
};