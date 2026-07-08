#include "services/biomes/biomeRepository.h"

#include <QObject>

namespace Cajander::Services {

BiomeRepository::BiomeRepository() {
    loadFromStorage();
}

const std::vector<Domain::Biome>& BiomeRepository::getAllBiomes() const {
    return m_biomes;
}

void BiomeRepository::loadFromStorage() {
    m_biomes.clear();

    // Временные mock-данные в стиле UX-driven, чтобы сразу видеть результат в интерфейсе
    m_biomes.push_back({10, QObject::tr("Tundra"), QObject::tr("Lichen and moss laminas, low shrubs")});
    m_biomes.push_back({20, QObject::tr("Taiga"), QObject::tr("Coniferous boreal forests")});
    m_biomes.push_back({30, QObject::tr("Mixed Forest"), QObject::tr("Deciduous and coniferous tree species")});
}

} // namespace Cajander::Services
