/**
 * @file ClassificationTab.h
 * @brief Configuration tab for setting up forest biome classification parameters.
 */

#pragma once

#include <QWidget>
#include <memory>

namespace Cajander::Gui {

class BiomeSelector; // Forward declaration within the same namespace

/**
 * @class ClassificationTab
 * @brief User interface tab responsible for managing biome classification settings.
 *
 * This tab integrates specialized selector widgets and reacts to user selection changes
 * to configure the active forest classification matrix.
 */
class ClassificationTab : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the classification settings tab.
     * @param parent Pointer to the parent widget container.
     */
    explicit ClassificationTab(QWidget* parent = nullptr);

    /**
     * @copydoc QWidget::~QWidget
     */
    ~ClassificationTab() override = default;

private slots:
    /**
     * @brief Slot triggered when a different biome item is selected.
     * @param index The QModelIndex pointing to the newly selected item in the model.
     */
    void onBiomeChanged(const QModelIndex& index);

private:
    BiomeSelector* m_biomeSelector{nullptr}; ///< Composite widget for selecting and viewing biomes.
};

} // namespace Cajander::Gui
