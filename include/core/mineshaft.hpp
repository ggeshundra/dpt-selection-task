#pragma once

#include <unordered_map>

#include "core/passage.hpp"
#include "core/room.hpp"

namespace expedition {

using Rooms = std::unordered_map<room_id_t, Room>;
using Passages = std::unordered_map<passage_id_t, Passage>;

struct Mineshaft {
    Rooms rooms;
    Passages passages;
    room_id_t entrance;
    value_t day_cost = 0;
};

} // namespace expedition
