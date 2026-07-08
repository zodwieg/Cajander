#include "biomerepository.h"
#include "ibiomestorage.h" // Подключаем интерфейс здесь

namespace Cajander::Services {

// Принимаем владение указателем через std::move
BiomeRepository::BiomeRepository(std::unique_ptr<IBiomeStorage> storage, QObject* parent) 
    : QObject(parent), m_storage(std::move(storage)) {
}

// Деструктор должен быть здесь, так как компилятору в этой точке 
// уже полностью известен размер и устройство типа IBiomeStorage
BiomeRepository::~BiomeRepository() = default;

void BiomeRepository::loadFromStorage() {
    m_biomes.clear();

    if (m_storage) {
        // Вызываем чтение из JSON и перемещаем вектор в наш кэш
        // std::move здесь избегает лишнего копирования тяжелого вектора строк
        m_biomes = std::move(m_storage->loadBiomes());
        emit biomesChanged();
    }
}

bool BiomeRepository::saveToStorage() const {
    if (m_storage) {
        return m_storage->saveBiomes(m_biomes);
    }
    return false;
}

} // namespace Cajander::Services
