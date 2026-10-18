#include <format>

#include "comm/action.hpp"

namespace expedition {

void ActionLook::print(std::ostream& os) const {
    std::print(os, "LOOK");
}

ActionMove::ActionMove(passage_id_t passage_id) noexcept : passage_id(std::move(passage_id)) {}

void ActionMove::print(std::ostream& os) const {
    std::print(os, "MOVE {}", passage_id);
}

ActionClear::ActionClear(passage_id_t passage_id) noexcept : passage_id(std::move(passage_id)) {}

void ActionClear::print(std::ostream& os) const {
    std::print(os, "CLEAR {}", passage_id);
}

void ActionEnd::print(std::ostream& os) const {
    std::print(os, "END");
}

std::ostream& operator<<(std::ostream& os, const Action& action) {
    action.print(os);
    return os;
}

} // namespace expedition
