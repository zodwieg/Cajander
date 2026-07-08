#pragma once

#include "services/biomes/ibiomestorage.h"
#include <QString>

namespace Cajander::Infrastructure {

class JsonBiomeStorage : public Services::IBiomeStorage {
public:
    JsonBiomeStorage();
    ~JsonBiomeStorage() override = default;

    std::vector<Domain::Biome> loadBiomes() override;
    bool saveBiomes(const std::vector<Domain::Biome>& biomes) override;

private:
    QString m_userFilePath;
    QString m_defaultFilePath;

    void ensureUserFileExists();
};

} // namespace Cajander::Infrastructure
