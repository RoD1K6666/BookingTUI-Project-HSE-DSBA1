#include "booking/persistence/StoreRepository.hpp"
#include "booking/persistence/StorageError.h"
#include "booking/persistence/AtomicFileWriter.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Booking.hpp"
#include "booking/domain/Hall.hpp"
#include "booking/domain/Types.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace booking::persistence {

Store StoreRepository::load(const std::string& path) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw StorageError("Не удалось открыть файл для чтения: " + path);
    }

    Store store;
    std::string line;

    std::getline(file, line);
    size_t sessions_count = std::stoul(line);
    for (size_t i = 0; i < sessions_count; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);

        std::string idStr, title, hallIdStr;
        std::getline(iss, idStr, '|');
        std::getline(iss, title, '|');
        std::getline(iss, hallIdStr, '|');

        auto hall = std::make_shared<domain::Hall>(10, 10);

        store.sessions.emplace_back(
            domain::SessionId(idStr),
            title,
            domain::HallId(hallIdStr),
            hall
        );
    }

    std::getline(file, line);
    size_t bookings_count = std::stoul(line);
    for (size_t i = 0; i < bookings_count; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);

        std::string bookingIdStr, sessionIdStr;
        int row, col;
        int seatTypeInt;
        long long price;

        char delim;
        std::getline(iss, bookingIdStr, '|');
        std::getline(iss, sessionIdStr, '|');
        iss >> row >> delim >> col >> delim;
        iss >> seatTypeInt >> delim >> price;

        store.bookings.emplace_back(
            domain::BookingId(bookingIdStr),
            domain::SessionId(sessionIdStr),
            domain::SeatPos{row, col},
            static_cast<domain::SeatType>(seatTypeInt),
            domain::Money(price)
        );
    }

    std::getline(file, line);
    size_t occupancy_count = std::stoul(line);
    for (size_t i = 0; i < occupancy_count; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);

        int sessionIdInt;
        char delim;
        iss >> sessionIdInt >> delim;

        size_t seats_count;
        iss >> seats_count;
        std::vector<int> seats(seats_count);
        for (size_t j = 0; j < seats_count; ++j) {
            iss >> delim >> seats[j];
        }
        store.occupancy[sessionIdInt] = seats;
    }

    return store;
}

void StoreRepository::save(const std::string& path, const Store& store) const {
    AtomicFileWriter writer{std::filesystem::path(path)};
    auto& file = writer.stream();

    file << store.sessions.size() << "\n";
    for (const auto& s : store.sessions) {
        file << s.id() << "|" << s.title() << "|" << s.hallId() << "\n";
    }

    file << store.bookings.size() << "\n";
    for (const auto& b : store.bookings) {
        file << b.id() << "|"
             << b.sessionId() << "|"
             << b.seatPos().row << "|" << b.seatPos().col << "|"
             << static_cast<int>(b.seatType()) << "|"
             << b.priceCents() << "\n";
    }

    file << store.occupancy.size() << "\n";
    for (const auto& [sessionId, seats] : store.occupancy) {
        file << sessionId << "|" << seats.size();
        for (int seat : seats) {
            file << "|" << seat;
        }
        file << "\n";
    }

    writer.commit();
}

}
