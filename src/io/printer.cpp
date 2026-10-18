#include <map>

#include "io/printer.hpp"

namespace io {

void print_world(std::ostream& os, const expedition::Mineshaft& mineshaft, const expedition::PassagesData& passages_data) {
    std::print(os, "WORLD\n");

    for (const auto& [id, passage] : std::map(mineshaft.passages.begin(), mineshaft.passages.end())) {
        std::print(os, "PASSAGE {} {} {} {} {}\n",
            id,
            passage.from,
            passage.to,
            passage.probability,
            passages_data.at(id).status == expedition::PassageExpeditionData::Status::ok ? "OK" : "BROKEN");
    }

    std::print(os, "END\n");
}

} // namespace io
