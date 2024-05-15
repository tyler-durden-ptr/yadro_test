#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <optional>

class Person {
public:
    enum class State {
        sit,
        came,
        gone
    };

private:
    std::optional<int64_t> tableId{};
    State state{State::came};

public:
    constexpr Person() noexcept = default;
    constexpr Person(const Person &) = default;
    constexpr Person(Person &&other) noexcept = default;
    constexpr Person &operator=(const Person &) = default;
    constexpr Person &operator=(Person &&) noexcept = default;

    constexpr const std::optional<int64_t> &getTableId() const noexcept {
        return tableId;
    }

    constexpr State getState() const noexcept {
        return state;
    }

    constexpr void SitToTheTable(int64_t newTableId) noexcept {
        state = State::sit;
        tableId.emplace(newTableId);
    }

    constexpr void Resit(int64_t newTableId) noexcept {
        tableId = newTableId;
    }

    constexpr void GoAway() noexcept {
        state = State::gone;
        tableId.reset();
    }
};
