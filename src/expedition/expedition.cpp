#include <format>
#include <stdexcept>

#include "expedition/expedition.hpp"

namespace expedition {

Expedition::Expedition(Mineshaft mineshaft, PassagesData passages_data)
    : m_mineshaft(std::move(mineshaft)), m_passages_data(std::move(passages_data)) {
    if (!m_mineshaft.rooms.contains(m_mineshaft.entrance)) {
        throw std::invalid_argument(std::format("Expedition: unknown entrance {}", m_mineshaft.entrance));
    }

    for (const auto& [id, passage] : m_mineshaft.passages) {
        if (!m_passages_data.contains(id)) {
            throw std::invalid_argument(std::format("Expedition: no state for passage {}", id));
        }
        if (!m_mineshaft.rooms.contains(passage.from) || !m_mineshaft.rooms.contains(passage.to)) {
            throw std::invalid_argument(std::format("Expedition: passage {} leads to unknown room", id));
        }
    }
}

Response Expedition::look(ExpeditionState& state, PassagesData& data) const {
    std::vector<Response::PassageView> passages;

    const auto& incident_passages = m_mineshaft.rooms.at(state.current_room_id).passages;
    for (const passage_id_t& passage_id : incident_passages) {
        PassageExpeditionData& passage_data = data.at(passage_id);
        passage_data.is_revealed = true;

        passages.push_back(Response::PassageView{
            .passage_id = passage_id,
            .to = m_mineshaft.passages.at(passage_id).other_end(state.current_room_id),
            .status = passage_data.status,
            });
    }

    return Response{
        .code = Response::Code::ok,
        .passages = std::move(passages)
    };
}

Response Expedition::move(ExpeditionState& state, PassagesData& data, const ActionMove& action) const {
    passage_id_t passage_id = action.passage_id;

    bool is_incident = m_mineshaft.rooms.at(state.current_room_id).passages.contains(passage_id);
    if (!is_incident) {
        return Response{ .code = Response::Code::forbidden };
    }

    PassageExpeditionData& passage_data = data.at(passage_id);
    passage_data.is_revealed = true;

    if (passage_data.status == PassageExpeditionData::Status::broken) {
        return Response{ .code = Response::Code::fail };
    }

    state.current_room_id = m_mineshaft.passages.at(passage_id).other_end(state.current_room_id);
    if (state.reached.insert(state.current_room_id).second) {
        state.collected += m_mineshaft.rooms.at(state.current_room_id).value;
    }

    return Response{ .code = Response::Code::ok };
}

Response Expedition::clear(ExpeditionState& state, PassagesData& data, const ActionClear& action) const {
    bool is_neighbor = m_mineshaft.rooms.at(state.current_room_id).passages.contains(action.passage_id);
    if (!is_neighbor) {
        return Response{ .code = Response::Code::forbidden };
    }

    PassageExpeditionData& passage_data = data.at(action.passage_id);

    if (!passage_data.is_revealed || passage_data.status != PassageExpeditionData::Status::broken) {
        return Response{ .code = Response::Code::forbidden };
    }

    passage_data.status = PassageExpeditionData::Status::ok;
    state.current_room_id = m_mineshaft.entrance;
    ++state.days;

    return Response{ .code = Response::Code::ok };
}

Response Expedition::act(ExpeditionState& state, PassagesData& data, const Action& action) const {
    if (dynamic_cast<const ActionLook*>(&action)) {
        return look(state, data);
    }

    if (const auto* action_move = dynamic_cast<const ActionMove*>(&action)) {
        return move(state, data, *action_move);
    }

    if (const auto* action_clear = dynamic_cast<const ActionClear*>(&action)) {
        return clear(state, data, *action_clear);
    }

    if (dynamic_cast<const ActionEnd*>(&action)) {
        return Response{ .code = Response::Code::ok };
    }

    return Response{ .code = Response::Code::forbidden };
}

ExpeditionResult Expedition::simulation(const ExpeditionAlgo& algo, uint32_t max_steps) const {
    ExpeditionState state(m_mineshaft);
    PassagesData data = m_passages_data;
    std::vector<ExpeditionResult::HistoryEntry> history;
    Response last_response;

    auto make_result = [&state, &history](std::string algorithm_error = "") {
        return ExpeditionResult{
            .collected = state.collected,
            .days = state.days,
            .profit = state.profit(),
            .algorithm_error = std::move(algorithm_error),
            .history = std::move(history),
        };
    };

    for (uint32_t step = 0; step < max_steps; ++step) {
        AlgoReturn action = algo(state, last_response);

        if (!action) {
            return make_result("Algorithm internal error: returned nullptr");
        }

        last_response = act(state, data, *action);
        bool algorithm_done = dynamic_cast<const ActionEnd*>(action.get()) != nullptr;
        history.emplace_back(std::move(action), last_response);

        if (algorithm_done) {
            return make_result();
        }
    }

    return make_result("Algorithm internal error: reached step limit");
}

} // namespace expedition
