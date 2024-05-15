#pragma once

#include <vector>
#include <queue>
#include <map>

#include "Table.h"
#include "Person.h"

class Club {
    std::vector<Table> tables;
    uint64_t numberOfAvailableTables;
    std::queue<std::string> waiters;
    std::string result;
    std::pair<int32_t, int32_t> openTime;
    std::pair<int32_t, int32_t> closeTime;
    std::map<std::string, Person> personByName;

public:
    explicit Club (size_t tableNumber, uint64_t hourPrice, std::string_view openTime, std::string_view closeTime);

    void ComeToClub(std::string_view time, std::string_view person, std::ostream& out);

    void SitToTable(std::string_view time, std::string_view person, int64_t tableId, std::ostream& out);

    void WaitInQueue(std::string_view time, std::string_view person, std::ostream& out);

    void GoAway(std::string_view time, std::string_view person, std::ostream& out);

    void Close(std::ostream& out);

private:
    void handleError(std::string_view time, std::string_view message, std::ostream& out);

    bool checkTime(std::string_view time) const;
};
