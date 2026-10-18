#include <map>
#include <random>

#include "expedition/generator.hpp"

namespace expedition {

PassagesData generate_passages_data(const Mineshaft& mineshaft, uint64_t seed) {
    std::mt19937_64 gen(seed);
    PassagesData passages_data;

    for (const auto& [id, passage] : std::map(mineshaft.passages.begin(), mineshaft.passages.end())) {
        passages_data[id].status = std::bernoulli_distribution(passage.probability)(gen)
            ? PassageExpeditionData::Status::ok
            : PassageExpeditionData::Status::broken;
    }

    return passages_data;
}

} // namespace expedition
