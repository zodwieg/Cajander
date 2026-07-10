#pragma once

#include "domain/biome.h"
#include <vector>

namespace Cajander::Services {

class IBiomeStorage {
public:
    virtual ~IBiomeStorage() = default;
    virtual std::vector<Domain::Biome> loadBiomes() = 0;
    virtual bool saveBiomes(const std::vector<Domain::Biome>& biomes) = 0;
    virtual bool saveBiomesTo(const std::vector<Domain::Biome>& biomes, const QString& filePath) = 0;
};

} // namespace Cajander::Services
