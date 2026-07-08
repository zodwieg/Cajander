#pragma once

#include "domain/biome.h"
#include <vector>
#include <memory>

namespace Cajander::Services {

class BiomeRepository {
public:
    BiomeRepository();
    ~BiomeRepository() = default;

    BiomeRepository(const BiomeRepository&) = delete;
    BiomeRepository& operator=(const BiomeRepository&) = delete;

    const std::vector<Domain::Biome>& getAllBiomes() const;

    void loadFromStorage();

private:
    std::vector<Domain::Biome> m_biomes;
};

} // namespace Cajander::Services
