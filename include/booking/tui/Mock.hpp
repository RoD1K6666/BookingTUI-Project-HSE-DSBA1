#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Врмененная "заглушка" чтобы показать проект в работе
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

inline std::tuple<
    std::unique_ptr<services::SequentialIdGenerator>,
    std::unique_ptr<services::SimplePricingService>,
    std::unique_ptr<services::BookingService>,
    std::vector<domain::Session>
>
makeMockData()
{
    auto ids     = std::make_unique<services::SequentialIdGenerator>();
    auto pricing = std::make_unique<services::SimplePricingService>(50000);
    auto service = std::make_unique<services::BookingService>(*pricing, *ids);

    std::vector<domain::Session> sessions;
    {
        // "Inception" — 4×5, два VIP места в первом ряду
        auto hall = std::make_shared<domain::Hall>(4, 5);
        hall->makeVip({0, 3});
        hall->makeVip({0, 4});
        sessions.emplace_back("S1", "Inception        | 2026-03-15 19:00", "H1", hall);
    }
    {
        // "Interstellar" — 3×4, весь задний ряд VIP
        auto hall = std::make_shared<domain::Hall>(3, 4);
        for (int c = 0; c < 4; ++c)
            hall->makeVip({2, c});
        sessions.emplace_back("S2", "Interstellar     | 2026-03-16 21:30", "H2", hall);
    }
    {
        // "Dune: Part Two" — большой зал 6×8, VIP последние два ряда
        auto hall = std::make_shared<domain::Hall>(6, 8);
        for (int c = 0; c < 8; ++c) {
            hall->makeVip({4, c});
            hall->makeVip({5, c});
        }
        sessions.emplace_back("S3", "Dune: Part Two   | 2026-03-17 20:00", "H3", hall);
    }
    {
        // "Oppenheimer" — 5×6, VIP центр среднего ряда
        auto hall = std::make_shared<domain::Hall>(5, 6);
        hall->makeVip({2, 2});
        hall->makeVip({2, 3});
        sessions.emplace_back("S4", "Oppenheimer      | 2026-03-18 18:30", "H1", hall);
    }
    {
        // "Чебурашка" — маленький уютный зал 3×5, VIP нет
        auto hall = std::make_shared<domain::Hall>(3, 5);
        sessions.emplace_back("S5", "Чебурашка        | 2026-03-15 12:00", "H4", hall);
    }
    {
        // "Мастер и Маргарита" — зал 5×7, весь последний ряд VIP
        auto hall = std::make_shared<domain::Hall>(5, 7);
        for (int c = 0; c < 7; ++c)
            hall->makeVip({4, c});
        sessions.emplace_back("S6", "Мастер и Маргарита | 2026-03-16 20:00", "H2", hall);
    }
    {
        // "Сталкер (реставрация)" — арт-кинотеатр, 4×4, все места VIP
        auto hall = std::make_shared<domain::Hall>(4, 4);
        for (int r = 0; r < 4; ++r)
            for (int c = 0; c < 4; ++c)
                hall->makeVip({r, c});
        sessions.emplace_back("S7", "Сталкер (4K)     | 2026-03-17 19:00", "H5", hall);
    }
    {
        // "Холоп 2" — большой зал 6×9, VIP задние два ряда
        auto hall = std::make_shared<domain::Hall>(6, 9);
        for (int c = 0; c < 9; ++c) {
            hall->makeVip({4, c});
            hall->makeVip({5, c});
        }
        sessions.emplace_back("S8", "Холоп 2          | 2026-03-18 21:00", "H3", hall);
    }
    {
        // "Брат (реставрация)" — небольшой зал 4×6, VIP два центральных места
        auto hall = std::make_shared<domain::Hall>(4, 6);
        hall->makeVip({1, 2});
        hall->makeVip({1, 3});
        hall->makeVip({2, 2});
        hall->makeVip({2, 3});
        sessions.emplace_back("S9", "Брат (4K)        | 2026-03-19 18:00", "H4", hall);
    }
    {
        // "Вызов" — средний зал 5×5, VIP диагональ
        auto hall = std::make_shared<domain::Hall>(5, 5);
        for (int i = 0; i < 5; ++i)
            hall->makeVip({i, i});
        sessions.emplace_back("S10", "Вызов            | 2026-03-20 20:30", "H2", hall);
    }

    return {std::move(ids), std::move(pricing), std::move(service), std::move(sessions)};
}

} // namespace booking::tui