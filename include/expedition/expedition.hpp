#pragma once

#include "expedition/expedition_data.hpp"

namespace expedition {

class Expedition {
    Mineshaft m_mineshaft;
    PassagesData m_passages_data;

    Response look(ExpeditionState& state, PassagesData& data) const;
    Response move(ExpeditionState& state, PassagesData& data, const ActionMove& action) const;
    Response clear(ExpeditionState& state, PassagesData& data, const ActionClear& action) const;
    Response act(ExpeditionState& state, PassagesData& data, const Action& action) const;

public:
    Expedition(Mineshaft mineshaft, PassagesData passages_data);

    ExpeditionResult simulation(const ExpeditionAlgo& algo, uint32_t max_steps = 1'000'000) const;
};

} // namespace expedition
