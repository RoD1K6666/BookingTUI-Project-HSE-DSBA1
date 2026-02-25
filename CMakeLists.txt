cmake_minimum_required(VERSION 3.20)
project(BookingTUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(booking_core
        src/domain/Booking.cpp
        src/domain/Hall.cpp
        src/domain/Session.cpp

        src/services/BookingService.cpp
        src/services/IdGenerator.cpp
        src/services/PricingService.cpp

        src/persistence/AtomicFileWriter.cpp
        src/persistence/StoreRepository.cpp

        src/tui/App.cpp
        src/tui/BookingsScreen.cpp
        src/tui/MainMenuScreen.cpp
        src/tui/SeatMapScreen.cpp
        src/tui/SessionsScreen.cpp
)

target_include_directories(booking_core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(booking_tui
        src/main.cpp
)

target_link_libraries(booking_tui PRIVATE booking_core)

# можно оставить, чтобы IDE точно видела include для exe
target_include_directories(booking_tui
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)