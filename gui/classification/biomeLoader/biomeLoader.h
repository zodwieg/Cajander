/**
 * @file BiomeLoader.h
 * @brief Helper controller responsible for importing forest biome configurations from JSON files.
 */

#pragma once

#include <QObject>
#include <QWidget>
#include <QString>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

/**
 * @class BiomeLoader
 * @brief File system bridge coordinating configuration file selections and storage mapping.
 *
 * This class instantiates temporary JSON serialization storages, parses structural schemas,
 * validates domain invariant constraints (e.g., non-empty sets), and triggers repository updates.
 */
class BiomeLoader : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the biome configuration loader.
     * @param repository Mutable reference to the central domain data repository.
     * @param parent Pointer to the parent widget used to anchor generated modal dialog windows.
     */
    explicit BiomeLoader(Services::BiomeRepository& repository, QWidget* parent = nullptr);

    /**
     * @copydoc QObject::~QObject
     */
    ~BiomeLoader() override = default;

public slots:
    /**
     * @brief Launches a native system modal file open dialogue box to pick a target JSON file.
     */
    void openFileDialog();

private:
    /**
     * @brief Parses and extracts schema models from the physical disk file path.
     * @param filePath Absolute path location pointing to the validation target.
     * @param errorMsg Output reference placeholder containing human-readable error reasons if parsing fails.
     * @return True if the scheme was completely loaded and pushed to domain caches, false otherwise.
     */
    bool loadJsonFromFile(const QString& filePath, QString& errorMsg);

    /**
     * @brief Renders a standardized modal QMessageBox warning alerting users to systemic execution anomalies.
     * @param text Detailed troubleshooting data to display inside the body block.
     */
    void showErrorDialog(const QString& text);

    Services::BiomeRepository& m_repository; ///< Reference to the back-end application storage controller.
    QWidget* m_parentWidget{nullptr};        ///< Borrowed viewport pointer for dialog spatial centering alignment.
};

} // namespace Cajander::Gui
