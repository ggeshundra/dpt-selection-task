#pragma once

#include <ostream>
#include <unordered_map>

#include "core/entity.hpp"

namespace expedition {

struct PassageExpeditionData {
    enum class Status {
        ok,
        broken
    } status = Status::ok;
    bool is_revealed = false;
};

std::ostream& operator<<(std::ostream& os, const PassageExpeditionData::Status& status);

using PassagesData = std::unordered_map<passage_id_t, PassageExpeditionData>;

} // namespace expedition
