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

/**
 * @brief Manages the lifecycle, caching, and CRUD operations for the active biome scheme.
 * @details Acts as an in-memory cache for biomes. Bridges the domain layer and persistent storage.
 * Inherits from QObject to provide reactive data updates to UI models via Qt Signals.
 */
class BiomeRepository : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructs the repository and injects the storage dependency.
     * @param storage Unique pointer to the storage implementation (ownership is transferred).
     * @param parent Optional pointer to the parent QObject for Qt's memory management tree.
     */
    explicit BiomeRepository(std::unique_ptr<IBiomeStorage> storage, QObject* parent = nullptr);

    /**
     * @brief Destructor. Overrides QObject destructor for clean resource deallocation.
     */
    ~BiomeRepository() override;

    /**
     * @brief Eagerly loads the biome scheme from the injected storage into memory.
     * @details If loading fails or the storage is empty, initializes a default blank scheme.
     * Triggers the biomesChanged() signal upon completion.
     */
    void loadFromStorage();

    /**
     * @brief Persists the current in-memory biome scheme back to the default storage.
     * @return true if successfully saved, false if storage is unavailable or serialization failed.
     */
    bool saveToStorage() const;

    /**
     * @brief Replaces the current in-memory scheme with a new one and persists it immediately.
     * @param newScheme The new biome scheme data to import. Taken by value to leverage move semantics.
     */
    void importScheme(Domain::BiomeScheme newScheme);

    /**
     * @brief Exports the current in-memory scheme to an external custom file path.
     * @param filePath The absolute target path for the export file.
     * @return true if export succeeded, false otherwise.
     */
    bool exportSchemeTo(const QString& filePath) const;

    /**
     * @brief Gets the human-readable name of the current biome scheme.
     * @return A const reference to the scheme name string.
     */
    const QString& getSchemeName() const;

    /**
     * @brief Updates the name of the current biome scheme.
     * @details Skips execution and does not trigger signals if the new name matches the old one.
     * Automatically triggers saveToStorage() and biomesChanged() on successful change.
     * @param newName The new name for the scheme.
     */
    void updateSchemeName(const QString& newName);

    /**
     * @brief Appends a new biome to the end of the current scheme.
     * @details Automatically saves changes to storage and triggers the biomesChanged() signal.
     * @param newBiome Const reference to the biome data structure to add.
     */
    void addBiome(const Domain::Biome& newBiome);

    /**
     * @brief Removes a biome from the scheme by its vector position.
     * @details Validates boundaries before deletion. Safe to call with invalid indices.
     * Automatically saves changes to storage and triggers biomesChanged() if a biome was removed.
     * @param index Zero-based index of the biome in the array.
     */
    void deleteBiome(std::size_t index);

    /**
     * @brief Replaces data of an existing biome at the specified index.
     * @details Validates boundaries before modification. Safe to call with invalid indices.
     * Automatically saves changes to storage and triggers biomesChanged() on success.
     * @param index Zero-based index of the biome to update.
     * @param updatedBiome The new biome data to apply.
     */
    void updateBiome(std::size_t index, const Domain::Biome& updatedBiome);

    /**
     * @brief Provides direct, read-only access to the internal list of biomes.
     * @return A const reference to the internal std::vector holding the biomes.
     */
    const std::vector<Domain::Biome>& getBiomes() const { return m_scheme.biomes; }

signals:
    /**
     * @brief Emitted whenever the biome collection or scheme metadata changes.
     * @details UI models (like BiomeModel) must listen to this signal to refresh the presentation layer.
     */
    void biomesChanged();

private:
    std::unique_ptr<IBiomeStorage> m_storage; ///< Managed instance of the persistent storage layer.
    Domain::BiomeScheme m_scheme; ///< Cached in-memory instance of the active biome scheme.
};

} // namespace Cajander::Services
