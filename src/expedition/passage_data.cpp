#include "expedition/passage_data.hpp"

namespace expedition {

std::ostream& operator<<(std::ostream& os, const PassageExpeditionData::Status& status) {
    switch (status) {
    case PassageExpeditionData::Status::ok:
        std::print(os, "OK");
        break;
    case PassageExpeditionData::Status::broken:
        std::print(os, "BROKEN");
        break;
    }
    return os;
}

} // namespace expedition
