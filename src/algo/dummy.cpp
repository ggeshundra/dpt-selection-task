#include <optional>
#include <unordered_map>

#include "algo/dummy.hpp"
#include "comm/response.hpp"
#include "expedition/expedition_data.hpp"
#include "expedition/passage_data.hpp"

namespace algo {

expedition::ExpeditionAlgo get_dummy() {
    return [](const expedition::ExpeditionState&, const expedition::Response&) -> expedition::AlgoReturn {
        return std::make_unique<expedition::ActionEnd>();
    };
}

} // namespace algo
