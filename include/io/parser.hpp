#pragma once

#include <istream>

#include "core/mineshaft.hpp"

namespace io {

expedition::Mineshaft parse(std::istream& input);

} // namespace io
