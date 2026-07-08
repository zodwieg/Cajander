#pragma once

#include <QWidget>

// Forward declarations (опережающие объявления), чтобы не тащить тяжелые заголовочные файлы в хэдер.
// Это ускоряет компиляцию и уменьшает связанность кода.
class QComboBox;
class QPushButton;
class QAbstractItemModel;

namespace Cajander::Gui {

    class BiomeSelector : public QWidget {
        Q_OBJECT

    public:
        // Принимаем модель через сырой указатель. В Qt управление временем жизни моделей 
        // обычно лежит на вызывающем коде (или родителе), а виджет просто её использует.
        explicit BiomeSelector(QAbstractItemModel* biomeModel, QWidget* parent = nullptr);
        ~BiomeSelector() override = default;

    signals:
        // Сигнал, который оповестит внешний мир (ClassificationTab), что пользователь выбрал другой биом.
        // Передаем QModelIndex, из которого потом можно вытащить ID или сам объект биома.
        void biomeChanged(const QModelIndex& index);
        
        // Сигнал о том, что нажата кнопка редактирования
        void editBiomesRequested();

    private slots:
        // Внутренние обработчики сигналов от Qt-виджетов
        void onComboBoxIndexChanged(int index);

    private:
        QComboBox* m_comboBox{nullptr};
        QPushButton* m_editButton{nullptr};
        QAbstractItemModel* m_model{nullptr};
    };
}
