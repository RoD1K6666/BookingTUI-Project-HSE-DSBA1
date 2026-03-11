#include "booking/tui/App.hpp"
#include "booking/tui/Mock.hpp"

// ─────────────────────────────────────────────────────────────────────────────
//  When teammates' persistence layer is ready, replace makeMockData() with:
//
//    #include "booking/persistence/StoreRepository.hpp"
//    #include "booking/services/SequentialIdGenerator.hpp"
//    #include "booking/services/SimplePricingService.hpp"
//    #include "booking/services/BookingService.hpp"
//
//    services::SequentialIdGenerator ids;
//    services::SimplePricingService  pricing;
//    services::BookingService        service(pricing, ids);
//
//    auto repo     = persistence::StoreRepository("data/");
//    auto sessions = repo.loadSessions();   // adjust to real API
//
//    booking::tui::App app(service, std::move(sessions));
//    app.run();
// ─────────────────────────────────────────────────────────────────────────────

#include <iostream>
#include <stdexcept>

int main() {
    try {
        // All objects live in this scope — service is passed by reference so
        // it must outlive App.
        auto [ids, pricing, service, sessions] = booking::tui::makeMockData();

        booking::tui::App app(*service, std::move(sessions));
        app.run();

    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "[FATAL] Unknown error.\n";
        return 2;
    }
    return 0;
}
