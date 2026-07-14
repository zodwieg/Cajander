#pragma once

#include <QDialog>
#include <memory>
#include <mutex>

class QTableView;
class QPushButton;
class QLineEdit;

namespace Cajander::Services { 
class BiomeRepository; 
}

namespace Cajander::Gui {

class BiomeTableModel;

/**
 * @class BiomeEditor
 * @brief Dialog window providing a comprehensive interface for managing and editing biome classification schemas.
 * 
 * Aggregates the table view, specific delegates, and data serialization controllers (import/export)
 * into a single user interface component.
 */
class BiomeEditor : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs the BiomeEditor dialog.
     * @param repository Reference to the service repository storing biome metadata and rules.
     * @param parent Optional parent QWidget for styling and window nesting.
     */
    explicit BiomeEditor(Services::BiomeRepository& repository, QWidget* parent = nullptr);
    
    /**
     * @brief Default virtual destructor.
     */
    ~BiomeEditor() override = default;

protected:
    /**
     * @copydoc QDialog::showEvent
     */
    void showEvent(QShowEvent* event) override;

    /**
     * @copydoc QDialog::eventFilter
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    /**
     * @brief Triggers the external file selection dialog to load an existing biome schema.
     */
    void onLoadBiomesClicked();

    /**
     * @brief Triggers the file export mechanism to serialize the current schema setup.
     */
    void onExportBiomesClicked();

    /**
     * @brief Synchronizes changes made to the scheme name text field back to the repository.
     */
    void onSchemeNameEdited();

    /**
     * @brief Fetches the active scheme name from the backend repository and displays it in the UI.
     */
    void updateSchemeNameFromRepo();

    /**
     * @brief Instantiates a blank biome configuration and appends it to the table model.
     */
    void onAddBiomeClicked();

    /**
     * @brief Refreshes structural UI layouts or status overlays inside custom cell views.
     */
    void updateTableWidgets();

private:
    /**
     * @brief Allocates and arranges child widgets, establishing parent-child links and layout structures.
     */
    void setupUi();

    Services::BiomeRepository& m_repository;    ///< Reference to the schema storage engine.
    BiomeTableModel* m_model{nullptr};          ///< Dedicated table model handle.
    
    QTableView* m_tableView{nullptr};            ///< Main viewer layout grid widget.
    QLineEdit* m_schemeNameEdit{nullptr};        ///< Text editor box for schema identification naming.
    QPushButton* m_loadButton{nullptr};          ///< Action button for triggering internal file loader tasks.
    QPushButton* m_exportButton{nullptr};        ///< Action button triggering external serialization workflows.
    QPushButton* m_closeButton{nullptr};         ///< UI button for clean close execution.
    QPushButton* m_addButton{nullptr};           ///< Controls appends for newly designed classification records.

    std::once_flag m_resourcesLoadedFlag;       ///< Thread-safe guard ensuring asset setup routines fire exactly once.
};

} // namespace Cajander::Gui
