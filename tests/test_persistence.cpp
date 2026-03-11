#include <gtest/gtest.h>
#include <filesystem>
#include <memory>

#include "booking/persistence/StoreRepository.hpp"
#include "booking/persistence/Store.hpp"
#include "booking/persistence/StorageError.h"
#include "booking/domain/Hall.hpp"
#include "booking/domain/Session.hpp"
#include "booking/domain/Booking.hpp"

using namespace booking::persistence;
using namespace booking::domain;

namespace {

// RAII: auto-deletes temp file after each test
struct TempFile {
    std::filesystem::path path;
    explicit TempFile(const std::string& name)
        : path(std::filesystem::temp_directory_path() / name) {}
    ~TempFile() { std::error_code ec; std::filesystem::remove(path, ec); }
    std::string str() const { return path.string(); }
};

Store makeStoreWithOneSession() {
    Store store;
    auto hall = std::make_shared<Hall>(10, 10);
    store.sessions.emplace_back("sess-1", "Test Session", "hall-1", hall);
    return store;
}

} // namespace

// ─── Save / Load roundtrip ────────────────────────────────────────────────────

TEST(StoreRepositoryTest, EmptyStoreSavesAndLoads) {
    TempFile tmp("test_empty.dat");
    StoreRepository repo;

    repo.save(tmp.str(), Store{});
    Store loaded = repo.load(tmp.str());

    EXPECT_TRUE(loaded.sessions.empty());
    EXPECT_TRUE(loaded.bookings.empty());
}

TEST(StoreRepositoryTest, SessionRoundtrip) {
    TempFile tmp("test_session.dat");
    StoreRepository repo;

    Store store = makeStoreWithOneSession();
    repo.save(tmp.str(), store);
    Store loaded = repo.load(tmp.str());

    ASSERT_EQ(loaded.sessions.size(), 1u);
    EXPECT_EQ(loaded.sessions[0].id(),     "sess-1");
    EXPECT_EQ(loaded.sessions[0].title(),  "Test Session");
    EXPECT_EQ(loaded.sessions[0].hallId(), "hall-1");
}

TEST(StoreRepositoryTest, BookingRoundtrip) {
    TempFile tmp("test_booking.dat");
    StoreRepository repo;

    Store store = makeStoreWithOneSession();
    store.bookings.emplace_back("b-1", "sess-1", SeatPos{2, 3}, SeatType::Regular, 500);
    repo.save(tmp.str(), store);
    Store loaded = repo.load(tmp.str());

    ASSERT_EQ(loaded.bookings.size(), 1u);
    EXPECT_EQ(loaded.bookings[0].id(),         "b-1");
    EXPECT_EQ(loaded.bookings[0].sessionId(),  "sess-1");
    EXPECT_EQ(loaded.bookings[0].seatPos(),    (SeatPos{2, 3}));
    EXPECT_EQ(loaded.bookings[0].seatType(),   SeatType::Regular);
    EXPECT_EQ(loaded.bookings[0].priceCents(), 500);
}

TEST(StoreRepositoryTest, VipBookingRoundtrip) {
    TempFile tmp("test_vip.dat");
    StoreRepository repo;

    Store store = makeStoreWithOneSession();
    store.bookings.emplace_back("b-vip", "sess-1", SeatPos{0, 0}, SeatType::Vip, 1500);
    repo.save(tmp.str(), store);
    Store loaded = repo.load(tmp.str());

    ASSERT_EQ(loaded.bookings.size(), 1u);
    EXPECT_EQ(loaded.bookings[0].seatType(),   SeatType::Vip);
    EXPECT_EQ(loaded.bookings[0].priceCents(), 1500);
}

TEST(StoreRepositoryTest, MultipleSessionsRoundtrip) {
    TempFile tmp("test_multi_sessions.dat");
    StoreRepository repo;

    Store store;
    auto hall = std::make_shared<Hall>(10, 10);
    store.sessions.emplace_back("s-1", "Morning Show", "h-1", hall);
    store.sessions.emplace_back("s-2", "Evening Show", "h-2", hall);
    repo.save(tmp.str(), store);
    Store loaded = repo.load(tmp.str());

    ASSERT_EQ(loaded.sessions.size(), 2u);
    EXPECT_EQ(loaded.sessions[0].id(), "s-1");
    EXPECT_EQ(loaded.sessions[1].id(), "s-2");
}

TEST(StoreRepositoryTest, MultipleBookingsRoundtrip) {
    TempFile tmp("test_multi_bookings.dat");
    StoreRepository repo;

    Store store = makeStoreWithOneSession();
    store.bookings.emplace_back("b-1", "sess-1", SeatPos{0, 0}, SeatType::Regular, 500);
    store.bookings.emplace_back("b-2", "sess-1", SeatPos{1, 1}, SeatType::Vip,     750);
    repo.save(tmp.str(), store);
    Store loaded = repo.load(tmp.str());

    ASSERT_EQ(loaded.bookings.size(), 2u);
    EXPECT_EQ(loaded.bookings[0].id(), "b-1");
    EXPECT_EQ(loaded.bookings[1].id(), "b-2");
}

// ─── Error handling ───────────────────────────────────────────────────────────

TEST(StoreRepositoryTest, LoadNonExistentFileThrows) {
    StoreRepository repo;
    EXPECT_THROW(repo.load("/nonexistent/path/no_file.dat"), StorageError);
}
