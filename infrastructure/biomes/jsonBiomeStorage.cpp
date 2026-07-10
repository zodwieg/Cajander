#include "jsonbiomestorage.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
#include <qgsapplication.h> 

namespace Cajander::Infrastructure {

JsonBiomeStorage::JsonBiomeStorage() {
    QString userDir = QgsApplication::qgisSettingsDirPath();
    userDir = QDir::cleanPath(userDir + "/python/plugins/cajander");
    m_userFilePath = userDir + "/biomes.json";
    m_defaultFilePath = userDir + "/default_biomes.json";
    m_isCustomMode = false;
}

// Реализация кастомного конструктора
JsonBiomeStorage::JsonBiomeStorage(const QString& customFilePath)
    : m_userFilePath(customFilePath)
    , m_isCustomMode(true) // В этом режиме дефолтные файлы не копируем
{
}

void JsonBiomeStorage::ensureUserFileExists() {
    if (m_isCustomMode || QFile::exists(m_userFilePath)) {
        return;
    }

    QFileInfo userFileInfo(m_userFilePath);
    QDir userDir = userFileInfo.dir();
    if (!userDir.exists()) {
        userDir.mkpath(".");
    }

    if (QFile::exists(m_defaultFilePath)) {
        QFile::copy(m_defaultFilePath, m_userFilePath);
        QFile::setPermissions(m_userFilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}

// Статический парсер из QJsonArray в вектор структур
std::vector<Domain::Biome> JsonBiomeStorage::parseJsonArray(const QJsonArray& array) {
    std::vector<Domain::Biome> result;
    result.reserve(array.size());

    for (const QJsonValue& value : array) {
        if (!value.isObject()) continue;
        
        QJsonObject obj = value.toObject();
        Domain::Biome biome;
        biome.code = obj["code"].toInt();
        biome.name = obj["name"].toString();
        biome.description = obj["description"].toString();

        result.push_back(biome);
    }
    return result;
}

// Статический сериализатор из вектора в QJsonArray
QJsonArray JsonBiomeStorage::serializeBiomes(const std::vector<Domain::Biome>& biomes) {
    QJsonArray array;
    for (const auto& biome : biomes) {
        QJsonObject obj;
        obj["code"] = biome.code;
        obj["name"] = biome.name;
        obj["description"] = biome.description;
        array.append(obj);
    }
    return array;
}

std::vector<Domain::Biome> JsonBiomeStorage::loadBiomes() {
    ensureUserFileExists();

    QFile file(m_userFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {}; 
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        return {}; 
    }

    // Переиспользуем статический метод
    return parseJsonArray(doc.array());
}

bool JsonBiomeStorage::saveBiomes(const std::vector<Domain::Biome>& biomes) {
    QJsonArray array = serializeBiomes(biomes);

    QJsonDocument doc(array);
    QFile file(m_userFilePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool JsonBiomeStorage::saveBiomesTo(const std::vector<Domain::Biome>& biomes, const QString& filePath) {
    if (filePath.isEmpty()) {
        return false;
    }

    QJsonArray array = serializeBiomes(biomes);
    QJsonDocument doc(array);
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

} // namespace Cajander::Infrastructure
