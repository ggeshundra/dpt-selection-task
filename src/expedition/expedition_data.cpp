#include "expedition/expedition_data.hpp"

namespace expedition {

ExpeditionState::ExpeditionState(const Mineshaft& mineshaft)
    : mineshaft(mineshaft), current_room_id(mineshaft.entrance) {
    reached.insert(current_room_id);
    collected = mineshaft.rooms.at(current_room_id).value;
}

profit_t ExpeditionState::profit() const noexcept {
    return static_cast<profit_t>(collected) - static_cast<profit_t>(days) * mineshaft.day_cost;
}

std::ostream& operator<<(std::ostream& os, const ExpeditionResult& result) {
    for (const auto& [action, response] : result.history) {
        os << "> " << *action << '\n';

        if (!dynamic_cast<const ActionEnd*>(action.get())) {
            os << response << '\n';
        }
    }

    if (!result.algorithm_error.empty()) {
        std::print(os, "ALGO ERROR: {}\n", result.algorithm_error);
    }

    std::print(os, "RESULT {} {} {}", result.collected, result.days, result.profit);
    return os;
}

} // namespace expedition
