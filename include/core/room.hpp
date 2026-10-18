#pragma once

#include <unordered_set>

#include "core/entity.hpp"

namespace expedition {

struct Room {
    value_t value = 0;
    std::unordered_set<passage_id_t> passages;

    Room();
    Room(value_t value);
};

} // namespace expedition
