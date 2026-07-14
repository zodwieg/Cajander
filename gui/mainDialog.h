/**
 * @file MainDialog.h
 * @brief Top-level container dialog for the CAJANDER QGIS plugin user interface.
 */

#pragma once

#include <QDialog>

class QTabWidget;

namespace Cajander::Gui {

/**
 * @class MainDialog
 * @brief Main window coordinating navigation between different plugin functional areas.
 *
 * Implements a tabbed interface containing data management, classification settings,
 * bulk processing, and help documentation components.
 */
class MainDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Strongly typed enumeration representing specific tab indices.
     */
    enum class TabIndex : int {
        Data = 0,           ///< Data management tab.
        Classification = 1, ///< Forest biome classification configuration tab.
        Processing = 2,     ///< Bulk raster processing execution tab.
        Help = 3            ///< Documentation and plugin help tab.
    };

    /**
     * @brief Constructs the main plugin dialog.
     * @param parent Pointer to the parent widget (typically QGIS main window).
     */
    explicit MainDialog(QWidget* parent = nullptr);

    /**
     * @copydoc QDialog::~QDialog
     */
    ~MainDialog() override = default;

    /**
     * @brief Switches the active view to the specified tab index.
     * @param index Zero-based target index within the internal QTabWidget.
     */
    void switchToTab(int index);

    /**
     * @brief Switches the active view specifically to the processing execution tab.
     */
    void openProcessingTab();

private:
    QTabWidget* m_tabWidget = nullptr; ///< Pointer to the underlying tab navigation container.
};

} // namespace Cajander::Gui
