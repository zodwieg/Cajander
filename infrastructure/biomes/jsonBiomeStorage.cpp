#include "jsonbiomestorage.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>
#include <qgsapplication.h> 

namespace Cajander::Infrastructure {

JsonBiomeStorage::JsonBiomeStorage() {
    // Leverage native QGIS paths API to isolate plugin profile configuration data
    QString userDir = QgsApplication::qgisSettingsDirPath();
    userDir = QDir::cleanPath(userDir + "/python/plugins/cajander");
    m_userFilePath = userDir + "/biomes.json";
    m_defaultFilePath = userDir + "/default_biomes.json";
    m_isCustomMode = false;
}

JsonBiomeStorage::JsonBiomeStorage(const QString& customFilePath)
    : m_userFilePath(customFilePath)
    , m_isCustomMode(true)
{
}

void JsonBiomeStorage::ensureUserFileExists() {
    // Guard clause: custom files should not be automatically generated or seeded
    if (m_isCustomMode || QFile::exists(m_userFilePath)) {
        return;
    }

    QFileInfo userFileInfo(m_userFilePath);
    QDir userDir = userFileInfo.dir();
    if (!userDir.exists()) {
        userDir.mkpath(".");
    }

    // Seed the user's operational file from a default baseline profile if available
    if (QFile::exists(m_defaultFilePath)) {
        QFile::copy(m_defaultFilePath, m_userFilePath);
        QFile::setPermissions(m_userFilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}

Domain::BiomeScheme JsonBiomeStorage::deserializeScheme(const QJsonObject& rootObj) {
    Domain::BiomeScheme scheme;
    
    scheme.name = rootObj.value("scheme_name").toString("Unnamed Scheme");
    QJsonArray array = rootObj.value("biomes").toArray();

    // Performance optimization: Pre-allocate capacity in vector to eliminate 
    // memory reallocations during push_back iterations
    scheme.biomes.reserve(array.size());

    for (const QJsonValue& value : array) {
        if (!value.isObject()) continue;
        
        QJsonObject obj = value.toObject();
        Domain::Biome biome;
        biome.code = obj["code"].toInt();
        biome.name = obj["name"].toString();
        biome.description = obj["description"].toString();

        if (obj.contains("color")) {
            biome.color = QColor(obj["color"].toString());
        }
        if (!biome.color.isValid()) {
            biome.color = QColor(Qt::white); // Default color fallback
        }

        scheme.biomes.push_back(biome);
    }
    return scheme;
}

QJsonObject JsonBiomeStorage::serializeScheme(const Domain::BiomeScheme& scheme) {
    QJsonObject rootObj;
    rootObj["scheme_name"] = scheme.name;

    QJsonArray array;
    for (const auto& biome : scheme.biomes) {
        QJsonObject obj;
        obj["code"] = biome.code;
        obj["name"] = biome.name;
        obj["description"] = biome.description;
        obj["color"] = biome.color.name(); // Converts QColor back to HEX format string
        array.append(obj);
    }
    
    rootObj["biomes"] = array;
    return rootObj;
}

std::optional<Domain::BiomeScheme> JsonBiomeStorage::loadScheme() {
    ensureUserFileExists();

    QFile file(m_userFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return std::nullopt; 
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    // Validate structural integrity and root data container type
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return std::nullopt; 
    }

    return deserializeScheme(doc.object());
}

bool JsonBiomeStorage::saveScheme(const Domain::BiomeScheme& scheme) {
    QJsonObject rootObj = serializeScheme(scheme);
    QJsonDocument doc(rootObj);
    QFile file(m_userFilePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool JsonBiomeStorage::saveSchemeTo(const Domain::BiomeScheme& scheme, const QString& filePath) {
    if (filePath.isEmpty()) {
        return false;
    }

    QJsonObject rootObj = serializeScheme(scheme);
    QJsonDocument doc(rootObj);
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

} // namespace Cajander::Infrastructure
