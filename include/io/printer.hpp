#pragma once

#include <ostream>

#include "core/mineshaft.hpp"
#include "expedition/passage_data.hpp"

namespace io {

void print_world(std::ostream& os, const expedition::Mineshaft& mineshaft, const expedition::PassagesData& passages_data);

} // namespace io
