#pragma once

#include <QString>
#include <QColor>
#include <vector>

/**
 * @namespace Cajander::Domain
 * @brief Contains core domain entities and data structures for the CAJANDER project.
 */
namespace Cajander::Domain {

/**
 * @brief Represents a single biome classification unit.
 * @details This structure holds the visual and textual data for a specific 
 * forest type, used during the classification and rendering of raster layers.
 */
struct Biome {
    int code{0}; ///< Unique identifier/code of the biome (the one that goes to the final raster as a pixel value).
    QString name; ///< Human-readable name of the biome.
    QString description; ///< Detailed description of the biome.
    QColor color{Qt::white}; ///< Color used to distinguish a biome and to render it in QGIS map canvas.

    /**
     * @brief Checks if two biomes are equal based on their unique code.
     * @param other The other biome to compare with.
     * @return true if the codes match, false otherwise.
     */
    bool operator==(const Biome& other) const {
        return code == other.code;
    }

    /**
     * @brief Checks if two biomes are not equal.
     * @param other The other biome to compare with.
     * @return true if the codes are different, false otherwise.
     */
    bool operator!=(const Biome& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Represents a collection of biomes forming a classification scheme.
 * @details Used to load, save, and manage predefined sets of forest types 
 * (e.g., "Cajander Taiga Scheme").
 */
struct BiomeScheme {
    QString name; ///< Name of the biome classification scheme.
    std::vector<Biome> biomes; ///< List of biomes included in this scheme.
};

} // namespace Cajander::Domain
