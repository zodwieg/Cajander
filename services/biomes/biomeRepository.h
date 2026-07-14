#pragma once

#include "domain/biome.h"
#include <vector>
#include <memory>
#include <QObject>
#include <QString>

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
    bool saveToStorage() const;

    void importScheme(Domain::BiomeScheme newScheme);
    bool exportSchemeTo(const QString& filePath) const;

    const QString& getSchemeName() const;
    void updateSchemeName(const QString& newName);

    void addBiome(const Domain::Biome& newBiome);
    void deleteBiome(std::size_t index);
    void updateBiome(std::size_t index, const Domain::Biome& updatedBiome);
    const std::vector<Domain::Biome>& getBiomes() const { return m_scheme.biomes; }

signals:
    void biomesChanged();

private:
    std::unique_ptr<IBiomeStorage> m_storage;
    Domain::BiomeScheme m_scheme;
};

} // namespace Cajander::Services
