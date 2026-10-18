#pragma once

#include <functional>
#include <memory>
#include <ostream>

#include "core/entity.hpp"

namespace expedition {

struct Action;
struct ExpeditionState;
struct Response;

using AlgoReturn = std::unique_ptr<Action>;
using ExpeditionAlgo = std::function<AlgoReturn(const ExpeditionState&, const Response& last_response)>;

struct Action {
    virtual ~Action() = default;
    virtual void print(std::ostream& os) const = 0;
};

struct ActionLook : public Action {
    void print(std::ostream& os) const override;
};

struct ActionMove : public Action {
    passage_id_t passage_id;

    explicit ActionMove(passage_id_t passage_id) noexcept;
    void print(std::ostream& os) const override;
};

struct ActionClear : public Action {
    passage_id_t passage_id;

    explicit ActionClear(passage_id_t passage_id) noexcept;
    void print(std::ostream& os) const override;
};

struct ActionEnd : public Action {
    void print(std::ostream& os) const override;
};

std::ostream& operator<<(std::ostream& os, const Action& action);

} // namespace expedition
