#pragma once

#include <QStyledItemDelegate>

namespace Cajander::Services { 
class BiomeRepository; 
}

namespace Cajander::Gui {

/**
 * @class BiomeItemDelegate
 * @brief Custom item delegate for specialized rendering and editing within the biome table views.
 * 
 * Handles custom cell appearance, including rendering interactive color indicators 
 * and action buttons (e.g., delete row). It intercepts standard user interactions 
 * to invoke specialized input widgets, such as QColorDialog for color selection.
 */
class BiomeItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    /**
     * @brief Constructs the BiomeItemDelegate.
     * @param repository Reference to the service repository for validation or synchronization tasks.
     * @param parent Optional parent QObject for memory management.
     */
    explicit BiomeItemDelegate(Services::BiomeRepository& repository, QObject* parent = nullptr);
    
    /**
     * @brief Default virtual destructor.
     */
    ~BiomeItemDelegate() override = default;

    /**
     * @copydoc QStyledItemDelegate::createEditor
     */
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    
    /**
     * @copydoc QStyledItemDelegate::setEditorData
     */
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    
    /**
     * @copydoc QStyledItemDelegate::setModelData
     */
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    
    /**
     * @copydoc QStyledItemDelegate::updateEditorGeometry
     */
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    /**
     * @copydoc QStyledItemDelegate::paint
     */
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    
    /**
     * @copydoc QStyledItemDelegate::editorEvent
     */
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    Services::BiomeRepository& m_repository; ///< Reference to the storage backend.

    /**
     * @brief Displays the standard color selection dialog and applies the result to the model.
     * @param model Pointer to the active item model.
     * @param option View styling options for the current cell.
     * @param index Model index representing the target cell.
     * @return True if the dialog was successfully opened and a color was chosen, false otherwise.
     */
    bool openColorDialog(QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

} // namespace Cajander::Gui
