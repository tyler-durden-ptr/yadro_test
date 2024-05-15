#pragma once

#include <cstdint>
#include <utility>

class Table {
public:
    enum class State {
        occupied,
        available,
    };

private:
    uint64_t money{0};
    std::pair<int32_t, int32_t> lastSatTime{0, 0};
    std::pair<int32_t, int32_t> occupiedTime{0, 0};
    State state{State::available};
    uint64_t hourPrice;

public:
    explicit constexpr Table(uint64_t hourPrice) noexcept : hourPrice(hourPrice) {}
    constexpr Table(const Table &) noexcept = default;
    constexpr Table(Table &&other) noexcept = default;
    constexpr Table &operator=(const Table &) = default;
    constexpr Table &operator=(Table &&) noexcept = default;

    [[nodiscard]] constexpr uint64_t getMoney() const noexcept  {
        return money;
    }

    [[nodiscard]] constexpr const std::pair<int32_t, int32_t> &getLastSatTime() const noexcept {
        return lastSatTime;
    }

    [[nodiscard]] constexpr const std::pair<int32_t, int32_t> &getOccupiedTime() const noexcept {
        return occupiedTime;
    }

    [[nodiscard]] constexpr State getState() const noexcept {
        return state;
    }

    [[nodiscard]] constexpr uint64_t getHourPrice() const noexcept {
        return hourPrice;
    }

    constexpr void SitToTheTable(std::pair<int32_t, int32_t> time) noexcept {
        lastSatTime = time;
        state = State::occupied;
    }

    constexpr void GoAway(std::pair<int32_t, int32_t> time) noexcept {
        auto diff = getTimeDiff(lastSatTime, time);
        money += hourPrice * (diff.second > 0 ? diff.first + 1 : diff.first);
        addOccupiedTime(lastSatTime, time);
        state = State::available;
    }

private:
    constexpr std::pair<int32_t, int32_t> getTimeDiff(std::pair<int32_t, int32_t> start, std::pair<int32_t, int32_t> end) const noexcept {
        auto diff = end;
        diff.second = end.second >= start.second ? end.second - start.second : 60 + end.second - start.second;
        diff.first = end.first >= start.first ? end.first - start.first : 24 + end.first - start.first;
        diff.first -= (end.second >= start.second ? 0 : 1);
        return diff;
    }

    constexpr void addOccupiedTime(std::pair<int32_t, int32_t> start, std::pair<int32_t, int32_t> end) noexcept {
        auto diff = getTimeDiff(start, end);
        if (diff.second + occupiedTime.second >= 60) {
            occupiedTime.first += 1 + diff.first;
            occupiedTime.second += diff.second - 60;
        } else {
            occupiedTime.first += diff.first;
            occupiedTime.second += diff.second;
        }
    }
};
