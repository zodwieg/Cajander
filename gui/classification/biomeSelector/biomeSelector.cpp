#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/plugincontext.h"
#include "gui/classification/biomeSelector/biomeModel.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QStyle>

namespace Cajander::Gui {

    BiomeSelector::BiomeSelector(QWidget* parent)
        : QWidget(parent)
    {
        // Вытаскиваем синглтон-модель прямо из контекста
        m_model = &PluginContext::instance().biomeModel(); 

        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0); 
        layout->setSpacing(6); 

        m_comboBox = new QComboBox(this);
        m_editButton = new QPushButton(this);

        m_editButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton)); 
        m_editButton->setToolTip(tr("Edit Biomes"));
        m_editButton->setFixedWidth(30); 

        // Привязываем модель к QComboBox
        if (m_model) {
            m_comboBox->setModel(m_model);
        }

        layout->addWidget(m_comboBox, 1); 
        layout->addWidget(m_editButton, 0); 

        connect(m_comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &BiomeSelector::onComboBoxIndexChanged);
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