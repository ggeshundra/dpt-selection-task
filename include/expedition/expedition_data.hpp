#pragma once

#include "comm/action.hpp"
#include "comm/response.hpp"
#include "core/mineshaft.hpp"

namespace expedition {

struct ExpeditionState {
    const Mineshaft& mineshaft;

    room_id_t current_room_id;
    std::unordered_set<room_id_t> reached;
    value_t collected = 0;
    uint32_t days = 0;

    explicit ExpeditionState(const Mineshaft& mineshaft);

    profit_t profit() const noexcept;
};

struct ExpeditionResult {
    value_t collected = 0;
    uint32_t days = 0;
    profit_t profit = 0;
    std::string algorithm_error;

    using HistoryEntry = std::pair<AlgoReturn, Response>;
    std::vector<HistoryEntry> history;
};

std::ostream& operator<<(std::ostream& os, const ExpeditionResult& result);

} // namespace expedition
