#pragma once

#include <QString>
#include <QColor>
#include <vector>

namespace Cajander::Domain {

struct Biome {
    int code{0};
    QString name;
    QString description;
    QColor color{Qt::white};
    bool operator==(const Biome& other) const {
        return code == other.code;
    }
    bool operator!=(const Biome& other) const {
        return !(*this == other);
    }
};

struct BiomeScheme {
    QString name;
    std::vector<Biome> biomes;
};

} // namespace Cajander::Domain
