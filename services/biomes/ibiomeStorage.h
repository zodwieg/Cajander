#pragma once

#include "domain/biome.h"
#include <optional>
#include <vector>

namespace Cajander::Services {

class IBiomeStorage {
public:
    virtual ~IBiomeStorage() = default;
    virtual std::optional<Domain::BiomeScheme> loadScheme() = 0;
    virtual bool saveScheme(const Domain::BiomeScheme& scheme) = 0;
    virtual bool saveSchemeTo(const Domain::BiomeScheme& scheme, const QString& filePath) = 0;
};

} // namespace Cajander::Services
