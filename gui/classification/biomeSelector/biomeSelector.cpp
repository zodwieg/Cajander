#include "gui/classification/biomeSelector/biomeSelector.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QStyle>

namespace Cajander::Gui {

    BiomeSelector::BiomeSelector(QAbstractItemModel* biomeModel, QWidget* parent)
        : QWidget(parent)
        , m_model(biomeModel) 
    {
        // 1. Инициализируем UI-элементы
        auto* layout = new QHBoxLayout(this);
        // Убираем внешние отступы, чтобы компонент аккуратно вставал в любые родительские Layout
        layout->setContentsMargins(0, 0, 0, 0); 
        layout->setSpacing(6); // Небольшой зазор между селектором и кнопкой

        m_comboBox = new QComboBox(this);
        m_editButton = new QPushButton(this);

        m_editButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)); 
        m_editButton->setToolTip(tr("Edit Biomes"));
        m_editButton->setFixedWidth(30); // Делаем кнопку компактной и квадратной

        // 2. Интегрируем строгое strict-решение: привязываем модель к QComboBox
        if (m_model) {
            m_comboBox->setModel(m_model);
        }

        // 3. Собираем Layout
        layout->addWidget(m_comboBox, 1); // Растягивается (weight = 1)
        layout->addWidget(m_editButton, 0); // Имеет фиксированную ширину (weight = 0)

        // 4. Коммуникация через современный синтаксис указателей на методы (C++11+)
        // QComboBox::currentIndexChanged имеет перегрузки в старых версиях Qt, 
        // но начиная с Qt 5.7/6 за основу взят вариант с int, работаем с ним.
        // Добавь #include <QComboBox> если вдруг забыл, но главное — синтаксис qOverload:
        connect(m_comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &BiomeSelector::onComboBoxIndexChanged);

        // Пробрасываем клик по кнопке наружу в виде нашего кастомного сигнала
        connect(m_editButton, &QPushButton::clicked, this, &BiomeSelector::editBiomesRequested);
    }

    void BiomeSelector::onComboBoxIndexChanged(int index) {
        if (!m_model || index < 0) return;

        // Превращаем int-индекс комбобокса в валидный QModelIndex нашей модели
        QModelIndex modelIndex = m_model->index(index, 0);
        if (modelIndex.isValid()) {
            emit biomeChanged(modelIndex);
        }
    }
}