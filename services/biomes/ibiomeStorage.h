#pragma once

#include "domain/biome.h"
#include <optional>
#include <vector>

namespace Cajander::Services {

/**
 * @brief Interface for persistent storage of biome classification schemes.
 * @details Defines the abstraction layer for loading and saving biome data. 
 * Implementations can handle various formats such as JSON, XML, or database storage.
 */
class IBiomeStorage {
public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~IBiomeStorage() = default;

    /**
     * @brief Loads the default biome classification scheme from the storage.
     * @return An std::optional containing the BiomeScheme if successfully loaded 
     * and parsed; std::nullopt otherwise (e.g., file not found or corrupted).
     */
    virtual std::optional<Domain::BiomeScheme> loadScheme() = 0;

    /**
     * @brief Saves the given biome scheme to the default storage path.
     * @param scheme The biome classification scheme to persist.
     * @return true if the scheme was successfully written to the default storage, 
     * false otherwise.
     */
    virtual bool saveScheme(const Domain::BiomeScheme& scheme) = 0;

    /**
     * @brief Saves the given biome scheme to a specific external file path.
     * @details Used for exporting schemes to custom locations requested by the user.
     * @param scheme The biome classification scheme to export.
     * @param filePath The absolute target path where the file should be saved.
     * @return true if the export operation succeeded, false otherwise.
     */
    virtual bool saveSchemeTo(const Domain::BiomeScheme& scheme, const QString& filePath) = 0;
};

} // namespace Cajander::Services
