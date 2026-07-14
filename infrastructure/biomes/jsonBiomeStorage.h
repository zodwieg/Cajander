#pragma once

#include "services/biomes/ibiomestorage.h"
#include <QString>
#include <QJsonObject>
#include <optional>

namespace Cajander::Infrastructure {

/**
 * @brief JSON-based implementation of the IBiomeStorage interface.
 * @details Handles serialization, deserialization, and persistent disk operations 
 * for biome classification schemes. Integrates with QGIS system paths for storing 
 * user configuration files.
 */
class JsonBiomeStorage : public Services::IBiomeStorage {
public:
    /**
     * @brief Constructs storage operating on standard QGIS profile paths.
     * @details Automatically determines the target path using QgsApplication 
     * settings and prepares fallback logic from a default seed template.
     */
    JsonBiomeStorage();

    /**
     * @brief Constructs storage operating on a specific custom file location.
     * @details Disables automatic fallback seed copy logic. Used for working with 
     * custom, independent configuration files.
     * @param customFilePath Absolute path to the custom JSON file.
     */
    explicit JsonBiomeStorage(const QString& customFilePath);

    /**
     * @brief Default virtual destructor. Cleans up internal structures.
     */
    ~JsonBiomeStorage() override = default;

    /**
     * @copydoc Services::IBiomeStorage::loadScheme()
     */
    std::optional<Domain::BiomeScheme> loadScheme() override;

    /**
     * @copydoc Services::IBiomeStorage::saveScheme()
     */
    bool saveScheme(const Domain::BiomeScheme& scheme) override;

    /**
     * @copydoc Services::IBiomeStorage::saveSchemeTo()
     */
    bool saveSchemeTo(const Domain::BiomeScheme& scheme, const QString& filePath) override;

    /**
     * @brief Deserializes a raw Qt JSON object into a domain BiomeScheme structure.
     * @param rootObj Valid Qt JSON object containing scheme data.
     * @return Fully parsed BiomeScheme structure. Invalid or missing fields are 
     * replaced with safe default values.
     */
    static Domain::BiomeScheme deserializeScheme(const QJsonObject& rootObj);

    /**
     * @brief Serializes a domain BiomeScheme structure into a Qt JSON representation.
     * @param scheme Domain scheme data to process.
     * @return Packed QJsonObject ready for file streaming or network transmission.
     */
    static QJsonObject serializeScheme(const Domain::BiomeScheme& scheme);

private:
    QString m_userFilePath; ///< Target path used for operational read/write operations.
    QString m_defaultFilePath; ///< Reference fallback path containing initial seed biomes.
    bool m_isCustomMode{false}; ///< Flag indicating if storage bypasses the standard QGIS profile directories.

    /**
     * @brief Guarantees that a valid JSON target exists for the default user path.
     * @details If the file does not exist, it verifies directories and replicates 
     * the static fallback scheme template (`default_biomes.json`) with active permissions.
     */
    void ensureUserFileExists();
};

} // namespace Cajander::Infrastructure
