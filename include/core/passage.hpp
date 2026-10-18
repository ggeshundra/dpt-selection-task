#pragma once

#include "core/entity.hpp"

namespace expedition {

struct Passage {
    room_id_t from;
    room_id_t to;
    probability_t probability = 0;

    Passage();
    Passage(room_id_t from, room_id_t to, probability_t probability);

    bool is_incident(const room_id_t& room_id) const noexcept;
    const room_id_t& other_end(const room_id_t& room_id) const;
};

} // namespace expedition
