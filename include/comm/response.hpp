#pragma once

#include <optional>
#include <ostream>
#include <vector>

#include "core/entity.hpp"
#include "expedition/passage_data.hpp"

namespace expedition {

struct Response {
    enum class Code {
        ok,
        fail,
        forbidden
    } code = Code::ok;

    struct PassageView {
        passage_id_t passage_id;
        room_id_t to;
        PassageExpeditionData::Status status;
    };

    std::optional<std::vector<PassageView>> passages = std::nullopt;
};

std::ostream& operator<<(std::ostream& os, const Response& response);

} // namespace expedition
