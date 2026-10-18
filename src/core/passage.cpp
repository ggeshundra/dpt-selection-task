#include <format>
#include <stdexcept>

#include "core/passage.hpp"

namespace expedition {

Passage::Passage() = default;

Passage::Passage(room_id_t from, room_id_t to, probability_t probability)
    : from(std::move(from)), to(std::move(to)), probability(probability) {
}

bool Passage::is_incident(const room_id_t& room_id) const noexcept {
    return from == room_id || to == room_id;
}

const room_id_t& Passage::other_end(const room_id_t& room_id) const {
    if (room_id == from) {
        return to;
    }
    if (room_id == to) {
        return from;
    }
    throw std::invalid_argument(std::format("Passage::other_end: not incident to {}, passage between {} and {}", room_id, from, to));
}

} // namespace expedition
