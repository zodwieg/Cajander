/**
 * @file BiomeSelector.h
 * @brief Composite widget for selecting forest biomes and managing classification schemes.
 */

#pragma once

#include <QWidget>
#include <memory>

class QComboBox;
class QPushButton;
class QLabel;

namespace Cajander::Gui {

class BiomeListModel;

/**
 * @class BiomeSelector
 * @brief UI component combining a biome dropdown menu with schema management controls.
 *
 * Provides a streamlined interface to switch active forest biomes, display the active 
 * classification scheme name, and trigger loading/editing sub-dialogs.
 */
class BiomeSelector : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the biome selector widget.
     * @param parent Pointer to the parent widget.
     */
    explicit BiomeSelector(QWidget* parent = nullptr);

    /**
     * @copydoc QWidget::~QWidget
     */
    ~BiomeSelector() override = default;

signals:
    /**
     * @brief Emitted when a user selects a different biome item in the dropdown.
     * @param index Valid QModelIndex pointing to the newly selected item inside BiomeListModel.
     */
    void biomeChanged(const QModelIndex& index);

    /**
     * @brief Emitted when a request to load a new biome scheme is triggered.
     */
    void loadBiomesRequested();

    /**
     * @brief Emitted when a request to open the biome metadata editor is triggered.
     */
    void editBiomesRequested();

protected: 
    /**
     * @copydoc QWidget::showEvent
     * @note Used for lazy initial resource allocation and theme-aware SVG icon styling.
     */
    void showEvent(QShowEvent* event) override;

private slots:
    /**
     * @brief Internal slot capturing dropdown index changes.
     * @param index Zero-based positional position within the combo box list.
     */
    void onComboBoxIndexChanged(int index);

    /**
     * @brief Refreshes the top header label text from the domain repository configuration.
     */
    void updateSchemeLabel();

private:
    /**
     * @brief Opens a file dialog to read external JSON biome scheme files.
     */
    void onLoadBiomesClicked();

    /**
     * @brief Opens a modal tabular editor for direct biome attribute overrides.
     */
    void onEditBiomesClicked();

    QComboBox* m_comboBox{nullptr};          ///< Dropdown container displaying biomes and color markers.
    QPushButton* m_loadButton{nullptr};      ///< Button triggering configuration JSON imports.
    QPushButton* m_editButton{nullptr};      ///< Button executing the modal schema matrix configuration.
    QLabel* m_schemeNameLabel{nullptr};      ///< Metadata label representing active categorization style.
    BiomeListModel* m_model{nullptr};        ///< Borrowed pointer to application-wide data adapter.

    bool m_resourcesLoaded = false;          ///< RAII latch preventing repetitive SVG layout redraws.
};

} // namespace Cajander::Gui
