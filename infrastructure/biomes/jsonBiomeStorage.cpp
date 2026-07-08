#include "jsonbiomestorage.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>

#include <qgsmessagelog.h>
#include <qgis.h>

// Внимание: Подключай заголовки QGIS только в .cpp файле инфраструктуры!
#include <qgsapplication.h> 

namespace Cajander::Infrastructure {

JsonBiomeStorage::JsonBiomeStorage() {
    // Формируем пути в стиле QGIS
    QString userDir = QgsApplication::qgisSettingsDirPath();
    userDir = QDir::cleanPath(userDir + "/python/plugins/cajander");
    m_userFilePath = userDir + "/biomes.json";
    
    // Дефолтный файл лежит рядом с DLL плагина (в папке деплоя)
    m_defaultFilePath = userDir + "/default_biomes.json";
}

void JsonBiomeStorage::ensureUserFileExists() {
    if (QFile::exists(m_userFilePath)) {
        return;
    }

    // Если папки в профиле нет, создаем её
    QFileInfo userFileInfo(m_userFilePath);
    QDir userDir = userFileInfo.dir();
    if (!userDir.exists()) {
        userDir.mkpath(".");
    }

    // Копируем дефолтный JSON из папки плагина в профиль пользователя
    if (QFile::exists(m_defaultFilePath)) {
        QFile::copy(m_defaultFilePath, m_userFilePath);
        // Снимаем флаг Read-Only, если он скопировался операционной системой
        QFile::setPermissions(m_userFilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}

std::vector<Domain::Biome> JsonBiomeStorage::loadBiomes() {
    ensureUserFileExists();

    std::vector<Domain::Biome> result;
    QFile file(m_userFilePath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return result; // Возвращаем пустой вектор, если файл не открылся
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        return result; // Битый JSON
    }

    QJsonArray array = doc.array();
    result.reserve(array.size()); // Оптимизация аллокации памяти в STL-векторе

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

bool JsonBiomeStorage::saveBiomes(const std::vector<Domain::Biome>& biomes) {
    QJsonArray array;
    for (const auto& biome : biomes) {
        QJsonObject obj;
        obj["code"] = biome.code;
        obj["name"] = biome.name;
        obj["description"] = biome.description;
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(m_userFilePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented)); // Красивый отступ для пользователя
    file.close();
    return true;
}

} // namespace Cajander::Infrastructure
