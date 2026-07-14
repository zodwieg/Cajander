#include "biomeItemDelegate.h"
#include "biomeTableModel.h"
#include "services/biomes/biomeRepository.h"
#include "domain/biome.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QColorDialog>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

namespace Cajander::Gui {

BiomeItemDelegate::BiomeItemDelegate(Services::BiomeRepository& repository, QObject* parent)
    : QStyledItemDelegate(parent)
    , m_repository(repository)
{}

void BiomeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == BiomeTableModel::ColorColumn) {
        painter->save();

        // Drawing standard cell background (selection, focus, row alternation)
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        opt.widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        QColor color = index.data(Qt::EditRole).value<QColor>();
        if (!color.isValid()) {
            color = Qt::transparent;
        }

        // Calculating geometry of the centered color square
        const int iconSize = 14; 
        QRect rect = option.rect;
        int x = rect.x() + (rect.width() - iconSize) / 2;
        int y = rect.y() + (rect.height() - iconSize) / 2;
        QRect colorRect(x, y, iconSize, iconSize);

        painter->fillRect(colorRect, color);

        // Drawing thin gray border around the color indicator
        painter->setPen(QColor(160, 160, 160));
        painter->drawRect(colorRect);

        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool BiomeItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (index.column() == BiomeTableModel::Columns::ColorColumn) {
        
        // Preventing launching of the default text editor with mouse click
        if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                return true; 
            }
        }

        // Opening color selection dialog when mouse left button is strictly up
        if (event->type() == QEvent::MouseButtonRelease) {
            auto* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                return openColorDialog(model, option, index);
            }
        }

        // Blocking the default space key behavior
        if (event->type() == QEvent::KeyPress) {
            auto* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Space) {
                return true; 
            }
        }

        // Opening color selection dialog when space button is strictly up
        if (event->type() == QEvent::KeyRelease) {
            auto* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Space) {
                return openColorDialog(model, option, index);
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

bool BiomeItemDelegate::openColorDialog(QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QColor currentColor = index.data(Qt::EditRole).value<QColor>();

    // Calling the modal dialog, parent is the table widget (using const_cast)
    QColor chosenColor = QColorDialog::getColor(
        currentColor, 
        const_cast<QWidget*>(option.widget), 
        tr("Select Biome Color")
    );
    
    if (chosenColor.isValid() && chosenColor != currentColor) {
        model->setData(index, chosenColor, Qt::EditRole);
    }
    
    return true;
}

QWidget* BiomeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == BiomeTableModel::CodeColumn) {
        auto* spinBox = new QSpinBox(parent);
        spinBox->setRange(0, 999999); 
        spinBox->setFrame(false);
        return spinBox;
    }
    
    // We dont' create an editor widget for the color, managing it with editorEvent
    if (index.column() == BiomeTableModel::ColorColumn) {
        return nullptr; 
    }

    if (index.column() == BiomeTableModel::NameColumn || index.column() == BiomeTableModel::DescriptionColumn) {
        auto* lineEdit = new QLineEdit(parent);
        lineEdit->setFrame(false);
        return lineEdit;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void BiomeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (index.column() == BiomeTableModel::CodeColumn) {
        auto* spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            spinBox->setValue(index.model()->data(index, Qt::EditRole).toInt());
            return;
        }
    }

    if (index.column() == BiomeTableModel::NameColumn || index.column() == BiomeTableModel::DescriptionColumn) {
        auto* lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            lineEdit->setText(index.model()->data(index, Qt::EditRole).toString());
            return;
        }
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void BiomeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (index.column() == BiomeTableModel::CodeColumn) {
        auto* spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) {
            const int newCode = spinBox->value();
            const int currentCode = model->data(index, Qt::EditRole).toInt();

            // Checking if biome code is unique in the whole repository before saving
            if (newCode != currentCode) {
                const auto& biomes = m_repository.getBiomes();
                bool codeExists = std::any_of(biomes.begin(), biomes.end(),
                    [newCode](const Domain::Biome& b) { return b.code == newCode; }
                );

                if (codeExists) {
                    QMessageBox::warning(editor->window(), tr("Validation Error"), 
                        tr("Biome code %1 already exists. Codes must be unique.").arg(newCode));
                    return; // If not valid, data is not saved and the cell remains in editing mode
                }
            }
            model->setData(index, newCode, Qt::EditRole);
            return;
        }
    }

    if (index.column() == BiomeTableModel::NameColumn) {
        auto* lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            const QString trimmedName = lineEdit->text().trimmed();
            if (trimmedName.isEmpty()) {
                QMessageBox::warning(editor->window(), tr("Validation Error"), 
                    tr("Biome name cannot be empty."));
                return; 
            }
            model->setData(index, trimmedName, Qt::EditRole);
            return;
        }
    }

    if (index.column() == BiomeTableModel::DescriptionColumn) {
        auto* lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            model->setData(index, lineEdit->text().trimmed(), Qt::EditRole);
            return;
        }
    }

    QStyledItemDelegate::setModelData(editor, model, index);
}

void BiomeItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(index);
    // Fitting the inline editor widget geometry into the cell borders
    editor->setGeometry(option.rect);
}

} // namespace Cajander::Gui
