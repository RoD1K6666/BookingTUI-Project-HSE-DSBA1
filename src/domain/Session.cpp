#include "booking/domain/Session.hpp"
#include "booking/domain/Errors.hpp"

namespace booking::domain {

    Session::Session(SessionId id, std::string title, HallId hallId, std::shared_ptr<const Hall> hall)
        : id_(std::move(id)),
          title_(std::move(title)),
          hallId_(std::move(hallId)),
          hall_(std::move(hall)) {
        if (!hall_) {
            throw BookingError("Session hall pointer is null");
        }
    }

    const SessionId& Session::id() const noexcept { return id_; }
    const std::string& Session::title() const noexcept { return title_; }
    const HallId& Session::hallId() const noexcept { return hallId_; }

    const Hall& Session::hall() const noexcept { return *hall_; }

    bool Session::isBooked(SeatPos pos) const {
        (void)hall().seatAt(pos);
        return booked_.contains(pos);
    }

    void Session::book(SeatPos pos) {
        (void)hall().seatAt(pos);
        if (booked_.contains(pos)) {
            throw SeatUnavailable(pos);
        }
        booked_.insert(pos);
    }

    void Session::unbook(SeatPos pos) {
        (void)hall().seatAt(pos);
        booked_.erase(pos);
    }

    const std::unordered_set<SeatPos, SeatPosHash>& Session::bookedSeats() const noexcept {
        return booked_;
    }

}