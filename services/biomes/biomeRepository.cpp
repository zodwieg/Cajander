#include "biomeRepository.h"
#include "ibiomeStorage.h"

namespace Cajander::Services {

BiomeRepository::BiomeRepository(std::unique_ptr<IBiomeStorage> storage, QObject* parent) 
    : QObject(parent), m_storage(std::move(storage)) {
    // std::move transfers ownership of the storage pointer from the caller to this repository.
}

BiomeRepository::~BiomeRepository() = default;

void BiomeRepository::loadFromStorage() {
    if (!m_storage) return;

    auto loadedScheme = m_storage->loadScheme();
    if (loadedScheme.has_value()) {
        // Efficiently swap data blocks without copying using move assignment
        m_scheme = std::move(loadedScheme.value());
    } else {
        // Fallback initialization if storage is completely empty or missing
        m_scheme = Domain::BiomeScheme{"New Scheme", {}};
    }
    emit biomesChanged();
}

void BiomeRepository::importScheme(Domain::BiomeScheme newScheme) {
    // newScheme was passed by value. We move it into m_scheme to avoid a deep copy.
    m_scheme = std::move(newScheme);
    saveToStorage();
    emit biomesChanged();
}

const QString& BiomeRepository::getSchemeName() const {
    return m_scheme.name;
}

void BiomeRepository::updateSchemeName(const QString& newName) {
    if (m_scheme.name == newName) {
        // Guard clause to prevent unnecessary disk I/O and UI redraws
        return;
    }
    m_scheme.name = newName;
    saveToStorage();
    emit biomesChanged();
}

void BiomeRepository::addBiome(const Domain::Biome& newBiome) {
    m_scheme.biomes.push_back(newBiome);
    saveToStorage();
    emit biomesChanged();
}

void BiomeRepository::deleteBiome(std::size_t index) {
    if (index >= m_scheme.biomes.size()) {
        return; // Bounds check guard
    }
    m_scheme.biomes.erase(m_scheme.biomes.begin() + index);
    saveToStorage();
    emit biomesChanged();
}

void BiomeRepository::updateBiome(std::size_t index, const Domain::Biome& updatedBiome) {
    if (index >= m_scheme.biomes.size()) {
        return; // Bounds check guard
    }
    m_scheme.biomes[index] = updatedBiome;
    saveToStorage();
    emit biomesChanged();
}

bool BiomeRepository::exportSchemeTo(const QString& filePath) const {
    if (filePath.isEmpty() || !m_storage) {
        return false;
    }
    return m_storage->saveSchemeTo(m_scheme, filePath);
}

bool BiomeRepository::saveToStorage() const {
    if (m_storage) {
        return m_storage->saveScheme(m_scheme);
    }
    return false;
}

} // namespace Cajander::Services
