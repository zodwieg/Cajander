#include "biomeRepository.h"
#include "ibiomeStorage.h"

namespace Cajander::Services {

BiomeRepository::BiomeRepository(std::unique_ptr<IBiomeStorage> storage, QObject* parent) 
    : QObject(parent), m_storage(std::move(storage)) {
}

BiomeRepository::~BiomeRepository() = default;

void BiomeRepository::loadFromStorage() {
    m_biomes.clear();

    if (m_storage) {
        m_biomes = std::move(m_storage->loadBiomes());
        emit biomesChanged();
    }
}

void BiomeRepository::importBiomes(std::vector<Domain::Biome> newBiomes) {
    m_biomes = std::move(newBiomes);
    saveToStorage();
    emit biomesChanged();
}

bool BiomeRepository::saveToStorage() const {
    if (m_storage) {
        return m_storage->saveBiomes(m_biomes);
    }
    return false;
}

} // namespace Cajander::Services
