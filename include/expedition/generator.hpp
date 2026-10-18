#pragma once

#include <cstdint>

#include "core/mineshaft.hpp"
#include "expedition/passage_data.hpp"

namespace expedition {

PassagesData generate_passages_data(const Mineshaft& mineshaft, uint64_t seed);

} // namespace expedition
