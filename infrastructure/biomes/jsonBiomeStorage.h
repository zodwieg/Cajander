#pragma once

#include "services/biomes/ibiomestorage.h"
#include <QString>
#include <QJsonObject>
#include <optional>

namespace Cajander::Infrastructure {

class JsonBiomeStorage : public Services::IBiomeStorage {
public:
    JsonBiomeStorage();
    explicit JsonBiomeStorage(const QString& customFilePath);
    ~JsonBiomeStorage() override = default;

    std::optional<Domain::BiomeScheme> loadScheme() override;
    bool saveScheme(const Domain::BiomeScheme& scheme) override;
    bool saveSchemeTo(const Domain::BiomeScheme& scheme, const QString& filePath) override;

    static Domain::BiomeScheme deserializeScheme(const QJsonObject& rootObj);
    static QJsonObject serializeScheme(const Domain::BiomeScheme& scheme);

private:
    QString m_userFilePath;
    QString m_defaultFilePath;
    bool m_isCustomMode{false};

    void ensureUserFileExists();
};

} // namespace Cajander::Infrastructure
