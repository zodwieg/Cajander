#pragma once

#include <QStyledItemDelegate>

namespace Cajander::Services { class BiomeRepository; }

namespace Cajander::Gui {

class BiomeItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit BiomeItemDelegate(Services::BiomeRepository& repository, QObject* parent = nullptr);
    ~BiomeItemDelegate() override = default;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    Services::BiomeRepository& m_repository;
    bool openColorDialog(QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

} // namespace Cajander::Gui
