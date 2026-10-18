#include <deque>
#include <functional>
#include <optional>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "algo/sampling.hpp"
#include "comm/response.hpp"
#include "expedition/expedition_data.hpp"
#include "expedition/passage_data.hpp"

namespace algo {

namespace {

using namespace expedition;

using Status = PassageExpeditionData::Status;

class SamplingAlgo {
    static constexpr int kSamples = 500;

    std::mt19937_64 m_gen;
    std::unordered_map<passage_id_t, Status> m_discovered;
    std::deque<passage_id_t> m_curr_route;
    std::optional<passage_id_t> m_to_clear;

    void parse_response(const Response& response) {
        if (response.passages) {
            for (const auto& view : *response.passages) {
                m_discovered[view.passage_id] = view.status;
            }
        }
    }

    bool is_unexplored(const ExpeditionState& state, const room_id_t& room_id) const {
        if (!state.reached.contains(room_id)) {
            return true;
        }
        for (const passage_id_t& passage_id : state.mineshaft.rooms.at(room_id).passages) {
            if (!m_discovered.contains(passage_id)) {
                return true;
            }
        }
        return false;
    }

    std::deque<passage_id_t> dfs(
        const ExpeditionState& state,
        const room_id_t& curr,
        const std::function<bool(const room_id_t&)>& condition,
        std::unordered_set<room_id_t>& seen_rooms) const {
        seen_rooms.insert(curr);

        const auto& incident_passages = state.mineshaft.rooms.at(curr).passages;
        for (const passage_id_t& passage_id : incident_passages) {
            const room_id_t& next_room_id = state.mineshaft.passages.at(passage_id).other_end(curr);
            bool is_passable = m_discovered.contains(passage_id) && m_discovered.at(passage_id) == Status::ok;
            if (!is_passable || seen_rooms.contains(next_room_id)) {
                continue;
            }
            if (condition(next_room_id)) {
                return { passage_id };
            }
            std::deque<passage_id_t> route = dfs(state, next_room_id, condition, seen_rooms);
            if (!route.empty()) {
                route.push_front(passage_id);
                return route;
            }
        }
        return {};
    }

    std::deque<passage_id_t> route_with_condition(
        const ExpeditionState& state,
        const std::function<bool(const room_id_t&)>& condition
    ) const {
        std::unordered_set<room_id_t> seen_rooms;
        return dfs(state, state.current_room_id, condition, seen_rooms);
    }

    value_t value_behind(
        const ExpeditionState& state,
        const room_id_t& curr,
        std::unordered_map<passage_id_t, bool>& sampled_world,
        std::unordered_set<room_id_t>& seen_rooms) {
        seen_rooms.insert(curr);
        value_t total_value = state.mineshaft.rooms.at(curr).value;

        const auto& incident_passages = state.mineshaft.rooms.at(curr).passages;
        for (const passage_id_t& passage_id : incident_passages) {
            const Passage& passage = state.mineshaft.passages.at(passage_id);
            const room_id_t& next_room_id = passage.other_end(curr);

            if (!sampled_world.contains(passage_id)) {
                sampled_world[passage_id] = m_discovered.contains(passage_id)
                    ? m_discovered.at(passage_id) == Status::ok
                    : std::bernoulli_distribution(passage.probability)(m_gen);
            }
            if (sampled_world.at(passage_id) && !seen_rooms.contains(next_room_id) && !state.reached.contains(next_room_id)) {
                total_value += value_behind(state, next_room_id, sampled_world, seen_rooms);
            }
        }
        return total_value;
    }

    profit_t expected_profit(
        const ExpeditionState& state,
        const passage_id_t& passage_id,
        const room_id_t& far_room_id) {
        profit_t total_value = 0;
        for (int i = 0; i < kSamples; ++i) {
            std::unordered_map<passage_id_t, bool> sampled_world{ { passage_id, true } };
            std::unordered_set<room_id_t> seen_rooms;
            total_value += value_behind(state, far_room_id, sampled_world, seen_rooms);
        }
        return total_value / kSamples - state.mineshaft.day_cost;
    }

    std::optional<passage_id_t> choose_passage_to_clear(const ExpeditionState& state) {
        std::optional<passage_id_t> best_passage_id;
        profit_t best_profit = 0;

        for (const auto& [passage_id, passage] : state.mineshaft.passages) {
            bool is_broken = m_discovered.contains(passage_id)
                && m_discovered.at(passage_id) == Status::broken;
            bool is_from_reached = state.reached.contains(passage.from);
            bool is_to_reached = state.reached.contains(passage.to);
            if (!is_broken || is_from_reached == is_to_reached) {
                continue;
            }

            const room_id_t& far_room_id = is_from_reached ? passage.to : passage.from;
            profit_t profit = expected_profit(state, passage_id, far_room_id);
            if (profit > best_profit) {
                best_profit = profit;
                best_passage_id = passage_id;
            }
        }
        return best_passage_id;
    }

public:
    explicit SamplingAlgo(uint64_t seed) : m_gen(seed) {}

    AlgoReturn next(const ExpeditionState& state, const Response& response) {
        parse_response(response);

        if (!m_curr_route.empty()) {
            passage_id_t passage_id = m_curr_route.front();
            m_curr_route.pop_front();
            return std::make_unique<ActionMove>(passage_id);
        }

        if (m_to_clear) {
            passage_id_t passage_id = *m_to_clear;
            m_to_clear.reset();
            m_discovered[passage_id] = Status::ok;
            return std::make_unique<ActionClear>(passage_id);
        }

        if (is_unexplored(state, state.current_room_id)) {
            return std::make_unique<ActionLook>();
        }

        m_curr_route = route_with_condition(
            state,
            [&](const room_id_t& room_id) { return is_unexplored(state, room_id); }
        );
        if (!m_curr_route.empty()) {
            return next(state, response);
        }

        m_to_clear = choose_passage_to_clear(state);
        if (!m_to_clear) {
            return std::make_unique<ActionEnd>();
        }

        const Passage& passage = state.mineshaft.passages.at(*m_to_clear);
        const room_id_t& near_room_id = state.reached.contains(passage.from) ? passage.from : passage.to;
        if (near_room_id != state.current_room_id) {
            m_curr_route = route_with_condition(
                state, [&](const room_id_t& room_id) { return room_id == near_room_id; }
            );
        }
        return next(state, response);
    }
};

} // namespace

expedition::ExpeditionAlgo get_sampling_algo(uint64_t seed) {
    return [algo = SamplingAlgo(seed)](
        const expedition::ExpeditionState& state,
        const expedition::Response& response) mutable {
        return algo.next(state, response);
    };
}

} // namespace algo
