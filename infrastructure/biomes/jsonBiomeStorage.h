#pragma once

#include "services/biomes/ibiomestorage.h"
#include <QString>
#include <QJsonArray>

namespace Cajander::Infrastructure {

class JsonBiomeStorage : public Services::IBiomeStorage {
public:
    JsonBiomeStorage();
    
    explicit JsonBiomeStorage(const QString& customFilePath);
    
    ~JsonBiomeStorage() override = default;

    std::vector<Domain::Biome> loadBiomes() override;
    bool saveBiomes(const std::vector<Domain::Biome>& biomes) override;

    static std::vector<Domain::Biome> parseJsonArray(const QJsonArray& array);
    static QJsonArray serializeBiomes(const std::vector<Domain::Biome>& biomes);

private:
    QString m_userFilePath;
    QString m_defaultFilePath;
    bool m_isCustomMode{false};

    void ensureUserFileExists();
};

} // namespace Cajander::Infrastructure
