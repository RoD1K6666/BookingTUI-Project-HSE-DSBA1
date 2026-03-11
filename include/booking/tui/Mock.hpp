#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Mock.hpp
//  Utility to build a ready-to-run App with in-memory demo data —
//  no real persistence layer needed.
//
//  Usage (in main.cpp while teammates' persistence isn't wired up yet):
//
//    auto [service, sessions] = booking::tui::makeMockData();
//    booking::tui::App app(*service, std::move(sessions));
//    app.run();
// ─────────────────────────────────────────────────────────────────────────────

#include "booking/services/BookingService.hpp"
#include "booking/services/PricingService.hpp"
#include "booking/services/SimplePricingService.hpp"
#include "booking/services/IdGenerator.hpp"
#include "booking/services/SequentialIdGenerator.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Hall.hpp"
#include "booking/domain/Types.hpp"

#include <memory>
#include <vector>
#include <tuple>

namespace booking::tui {

// Returns a BookingService (heap-allocated so it outlives the caller's scope)
// and a list of demo sessions.
inline std::tuple<
    std::unique_ptr<services::SequentialIdGenerator>,
    std::unique_ptr<services::SimplePricingService>,
    std::unique_ptr<services::BookingService>,
    std::vector<domain::Session>
>
makeMockData()
{
    // ── Id generator & pricing (owned here, passed by ref to service) ──────
    auto ids     = std::make_unique<services::SequentialIdGenerator>();
    auto pricing = std::make_unique<services::SimplePricingService>();

    auto service = std::make_unique<services::BookingService>(*pricing, *ids);

    // ── Demo sessions ──────────────────────────────────────────────────────
    std::vector<domain::Session> sessions;

    {
        // "Inception" — 4 rows × 5 cols, seats (0,3) and (0,4) are VIP
        auto hall = std::make_shared<domain::Hall>(4, 5);
        hall->makeVip({0, 3});
        hall->makeVip({0, 4});
        sessions.emplace_back("S1", "Inception  | 2026-03-15 19:00", "H1", hall);
    }
    {
        // "Interstellar" — 3 rows × 4 cols, back row is VIP
        auto hall = std::make_shared<domain::Hall>(3, 4);
        for (int c = 0; c < 4; ++c)
            hall->makeVip({2, c});
        sessions.emplace_back("S2", "Interstellar | 2026-03-16 21:30", "H2", hall);
    }

    return {std::move(ids), std::move(pricing), std::move(service), std::move(sessions)};
}

} // namespace booking::tui
