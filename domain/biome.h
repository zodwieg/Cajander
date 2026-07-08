#pragma once

#include <QString>

namespace Cajander::Domain {

struct Biome {
    int code{0};
    QString name;
    QString description;
    bool operator==(const Biome& other) const {
        return code == other.code;
    }
    bool operator!=(const Biome& other) const {
        return !(*this == other);
    }
};

} // namespace Cajander::Domain
