#include "headers/Club.h"
#include "headers/Table.h"
#include "headers/Person.h"

#include <utility>
#include <stdexcept>
#include <string>
#include <string_view>
#include <format>

template <typename CharType>
constexpr int32_t getDigit(CharType ch) noexcept {
    return ch - '0';
}

constexpr std::pair<int32_t, int32_t> parseTime(std::string_view time) {
    if (time.size() != 5 || time[2] != ':' || !std::isdigit(time[0]) || !std::isdigit(time[1]) ||
        !std::isdigit(time[3]) || !std::isdigit(time[4])) {
        throw std::invalid_argument(time.data());
    }
    int32_t hour = getDigit(time[0]) * 10 + getDigit(time[1]);
    int32_t minutes = getDigit(time[3]) * 10 + getDigit(time[4]);
    if (hour < 0 || hour > 24 || minutes < 0 || minutes > 60) {
        throw std::invalid_argument(time.data());
    }
    return {hour, minutes};
}

std::string getStringTime(std::pair<int32_t, int32_t> time) {
    return std::format("{:02}:{:02}", time.first, time.second);
}

Club::Club(size_t tableNumber, uint64_t hourPrice, std::string_view openTime, std::string_view closeTime)
    : tables(tableNumber, Table(hourPrice)), numberOfAvailableTables(tableNumber),
      openTime(parseTime(openTime)), closeTime(parseTime(closeTime)), personByName() {}

bool Club::checkTime(std::string_view time) const {
    auto [hours, minutes] = parseTime(time);
    int32_t openTimeMin = openTime.first * 60 + openTime.second;
    int32_t closeTimeMin = closeTime.first * 60 + closeTime.second;
    int32_t currentTime = hours * 60 + minutes;

    if (openTimeMin <= closeTimeMin) {
        return currentTime >= openTimeMin && currentTime < closeTimeMin;
    } else {
        return currentTime >= openTimeMin || currentTime < closeTimeMin;
    }
}

void Club::ComeToClub(std::string_view time, std::string_view person, std::ostream &out) {
    if (personByName.contains(person.data())) {
        handleError(time, "YouShallNotPass", out);
    } else if (!checkTime(time)) {
        handleError(time, "NotOpenYet", out);
    } else {
        personByName[person.data()] = {};
    }
}


void Club::SitToTable(std::string_view time, std::string_view person, int64_t tableId, std::ostream &out) {
    tableId -= 1;
    if (!personByName.contains(person.data())) {
        handleError(time, "ClientUnknown", out);
    } else if (tables[tableId].getState() == Table::State::occupied) {
        handleError(time, "PlaceIsBusy", out);
    } else {
        Person& personData = personByName[person.data()];
        if (personData.getTableId().has_value()) {
            Table& table = tables[personData.getTableId().value()];
            table.GoAway(parseTime(time));
            table = tables[tableId];
            personData.Resit(tableId);
        } else {
            personData.SitToTheTable(tableId);
            --numberOfAvailableTables;
        }
        tables[tableId].SitToTheTable(parseTime(time));
    }
}
void Club::WaitInQueue(std::string_view time, std::string_view person, std::ostream &out) {
    if (numberOfAvailableTables > 0) {
        handleError(time, "ICanWaitNoLonger!", out);
    } else {
        if (waiters.size() > tables.size()) {
            out << std::format("{} {} {}", time, 11, person) << std::endl;
        } else {
            waiters.push(person.data());
        }
    }
}
void Club::GoAway(std::string_view time, std::string_view person, std::ostream &out) {
    if (!personByName.contains(person.data())) {
        handleError(time, "ClientUnknown!", out);
    } else {
        auto& personData = personByName[person.data()];
        if (personData.getTableId().has_value()) {
            int64_t tableId = personData.getTableId().value();
            Table& table = tables[tableId];
            table.GoAway(parseTime(time));
            personByName.erase(person.data());
            if (waiters.empty()) {
                ++numberOfAvailableTables;
                return;
            }
            std::string name = waiters.front();
            waiters.pop();
            auto& nextPerson = personByName[name];
            nextPerson.SitToTheTable(tableId);
            table.SitToTheTable(parseTime(time));
            out << std::format("{} {} {} {}", time, 12, name, tableId + 1) << std::endl;
        }
    }
}

void Club::Close(std::ostream& out) {
    for (auto& [personName, personData] : personByName) {
        if (personData.getState() == Person::State::sit) {
            tables[personData.getTableId().value()].GoAway(closeTime);
            out << std::format("{} {} {}", getStringTime(closeTime), 11, personName) << std::endl;
            personData.GoAway();
        }
    }
    out << getStringTime(closeTime) << std::endl;
    for (size_t i = 0; i < tables.size(); i++) {
        Table& table = tables[i];
        if (table.getOccupiedTime() != std::pair{0, 0}) {
            out << std::format("{} {} {}", i + 1, table.getMoney(), getStringTime(table.getOccupiedTime())) << std::endl;
        }
    }
}

void Club::handleError(std::string_view time, std::string_view message, std::ostream &out) {
    out << std::format("{} {} {}", time, 13, message) << std::endl;
}
