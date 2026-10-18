#include "comm/response.hpp"

namespace expedition {

std::ostream& operator<<(std::ostream& os, const Response& response) {
    if (response.code == Response::Code::forbidden) {
        std::print(os, "FORBIDDEN");
        return os;
    }

    if (response.passages) {
        for (std::size_t i = 0; i < response.passages->size(); ++i) {
            const auto& passage = response.passages->at(i);
            std::print(os, "{}PATH {} {} {}",
                i > 0 ? "\n" : "",
                passage.passage_id,
                passage.to,
                passage.status == PassageExpeditionData::Status::ok ? "OK" : "BROKEN");
        }
        return os;
    }

    std::print(os, "{}", response.code == Response::Code::ok ? "OK" : "FAIL");
    return os;
}

} // namespace expedition
