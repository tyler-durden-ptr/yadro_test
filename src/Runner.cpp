#include "headers/Runner.h"
#include "headers/Club.h"

#include <cstdint>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <cctype>

template<typename... Args>
void fillWithCheck(std::string_view view, Args &&...args) {
    std::stringstream ss(view.data());
    (ss >> ... >> args);
    if (ss.fail() && !ss.eof()) {
        throw std::invalid_argument(view.data());
    }
}

template<>
void fillWithCheck(std::string_view view, std::string &eventTime, int32_t &eventId, std::string &personName, int64_t& tableId) {
    std::stringstream ss(view.data());
    ss >> eventTime >> eventId >> personName;
    if (eventId == 2) {
        ss >> tableId;
    }
    while (!ss.eof()) {
        char ch;
        ss >> ch;
        if (!std::isspace(ch)) {
            throw std::invalid_argument(view.data());
        }
    }
    if (ss.fail() && !ss.eof()) {
        throw std::invalid_argument(view.data());
    }
}

template<typename... Args>
void readLineAndFillVars(std::istream &input, std::string& tempLine, Args &&...args) {
    std::getline(input, tempLine);
    fillWithCheck(tempLine, std::forward<Args>(args)...);
}

void runner::run(std::istream &input, std::ostream &out) {
    size_t tableNumber;
    std::string openTime, closeTime, tempLine;
    uint64_t hourPrice;
    try {
        readLineAndFillVars(input, tempLine, tableNumber);
        readLineAndFillVars(input, tempLine, openTime, closeTime);
        readLineAndFillVars(input, tempLine, hourPrice);

        Club club(tableNumber, hourPrice, openTime, closeTime);
        std::stringstream tryToParse;

        int64_t tableId;
        std::string eventTime, personName;
        int32_t eventId;// Do not change int32_t, because there is a function specialization for it

        tryToParse << openTime << std::endl;

        while (true) {
            readLineAndFillVars(input, tempLine, eventTime, eventId, personName, tableId);
            if (input.eof()) {
                break;
            }
            tryToParse << tempLine << std::endl;
            switch (eventId) {
                case 1:
                    club.ComeToClub(eventTime, personName, tryToParse);
                    break;
                case 2:
                    club.SitToTable(eventTime, personName, tableId, tryToParse);
                    break;
                case 3:
                    club.WaitInQueue(eventTime, personName, tryToParse);
                    break;
                case 4:
                    club.GoAway(eventTime, personName, tryToParse);
                    break;
            }
        }
        out << tryToParse.str();
        club.Close(out);
    } catch (std::invalid_argument& ignored) {
        out << tempLine;
    }
    out.flush();
}
