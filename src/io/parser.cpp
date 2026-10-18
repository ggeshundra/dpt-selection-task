#include <format>
#include <stdexcept>

#include "io/parser.hpp"

namespace io {

expedition::Mineshaft parse(std::istream& input) {
    expedition::Mineshaft res;
    std::string keyword;

    if (!(input >> keyword) || keyword != "MAP") {
        throw std::invalid_argument("parse: expected MAP");
    }

    while (input >> keyword && keyword != "END") {
        if (keyword == "day_cost") {
            input >> res.day_cost;
        } else if (keyword == "entrance") {
            input >> res.entrance;
        } else if (keyword == "vertex") {
            expedition::room_id_t room_id;
            expedition::value_t value = 0;
            input >> room_id >> value;
            res.rooms.emplace(room_id, expedition::Room(value));
        } else if (keyword == "edge") {
            expedition::passage_id_t passage_id;
            expedition::room_id_t from;
            expedition::room_id_t to;
            expedition::probability_t probability = 0;
            input >> passage_id >> from >> to >> probability;
            res.rooms.at(from).passages.insert(passage_id);
            res.rooms.at(to).passages.insert(passage_id);
            res.passages.emplace(passage_id, expedition::Passage(from, to, probability));
        } else {
            throw std::invalid_argument(std::format("parse: unknown keyword {}", keyword));
        }

        if (!input) {
            throw std::invalid_argument(std::format("parse: bad {} line", keyword));
        }
    }

    if (keyword != "END") {
        throw std::invalid_argument("parse: expected END");
    }

    return res;
}

} // namespace io
