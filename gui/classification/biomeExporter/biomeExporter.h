/**
 * @file BiomeExporter.h
 * @brief Helper controller responsible for exporting active forest biome configurations to JSON files.
 */

#pragma once

#include <QObject>
#include <QWidget>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

/**
 * @class BiomeExporter
 * @brief File system bridge coordinating configuration serialization and data export transactions.
 *
 * This class validates the state of the active biome schema data, sanitizes output file names,
 * and delegates structural formatting execution down to the underlying repository services.
 */
class BiomeExporter : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the biome configuration exporter.
     * @param repository Valid non-null pointer to the application data repository.
     * @param parent Pointer to the parent QObject owner (safely converted internally to QWidget for dialog parenting).
     * @pre repository != nullptr
     */
    explicit BiomeExporter(Services::BiomeRepository* repository, QObject* parent = nullptr);

    /**
     * @copydoc QObject::~QObject
     */
    ~BiomeExporter() override = default;

    /**
     * @brief Launches a native system modal file save dialogue box to choose target destination.
     */
    void openSaveDialog();

private:
    Services::BiomeRepository* m_repository{nullptr}; ///< Borrowed pointer to data persistence layers.
    QWidget* m_parentWidget{nullptr};                 ///< Resolved viewport container pointer for modal anchoring.
};

} // namespace Cajander::Gui
