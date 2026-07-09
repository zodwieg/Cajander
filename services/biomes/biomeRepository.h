#pragma once

#include "domain/biome.h"
#include <vector>
#include <memory>
#include <QObject>

namespace Cajander::Services {
    class IBiomeStorage;
}

namespace Cajander::Services {

class BiomeRepository : public QObject {
    Q_OBJECT
public:
    explicit BiomeRepository(std::unique_ptr<IBiomeStorage> storage, QObject* parent = nullptr);

    ~BiomeRepository() override;

    void loadFromStorage();

    void importBiomes(std::vector<Domain::Biome> newBiomes);

    bool saveToStorage() const;

    const std::vector<Domain::Biome>& getBiomes() const { return m_biomes; }

signals:
    void biomesChanged();

private:
    std::unique_ptr<IBiomeStorage> m_storage;
    std::vector<Domain::Biome> m_biomes;
};

} // namespace Cajander::Services
