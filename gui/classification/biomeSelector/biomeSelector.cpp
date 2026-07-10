#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/plugincontext.h"
#include "gui/classification/biomeSelector/biomeModel.h"
#include "gui/utils/uiUtils.h"

// Инклуды сервисов и тяжелых окон теперь живут ТОЛЬКО в .cpp
#include "gui/classification/biomeLoader/biomeLoader.h"
#include "gui/classification/biomeEditor/biomeEditor.h"
#include "services/biomes/biomeRepository.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

namespace Cajander::Gui {

    BiomeSelector::BiomeSelector(QWidget* parent)
        : QWidget(parent)
    {
        m_model = &PluginContext::instance().biomeModel(); 

        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0); 
        layout->setSpacing(6); 

        m_comboBox = new QComboBox(this);

        m_loadButton = new QPushButton(this);
        m_loadButton->setToolTip(tr("Load Biomes JSON"));
        m_loadButton->setFixedSize(30, 24);

        m_editButton = new QPushButton(this);
        m_editButton->setToolTip(tr("Edit Biomes"));
        m_editButton->setFixedSize(30, 24);

        if (m_model) {
            m_comboBox->setModel(m_model);
        }

        layout->addWidget(m_comboBox, 1); 
        layout->addWidget(m_loadButton, 0); 
        layout->addWidget(m_editButton, 0);

        // Полная симметрия и простота в коннектах
        connect(m_comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &BiomeSelector::onComboBoxIndexChanged);
        connect(m_loadButton, &QPushButton::clicked, this, &BiomeSelector::onLoadBiomesClicked);
        connect(m_editButton, &QPushButton::clicked, this, &BiomeSelector::onEditBiomesClicked);
    }

    void BiomeSelector::onComboBoxIndexChanged(int index) {
        if (!m_model || index < 0) return;

        QModelIndex modelIndex = m_model->index(index, 0);
        if (modelIndex.isValid()) {
            emit biomeChanged(modelIndex);
        }
    }

    // Лоадер: создался, отработал, удалился
    void BiomeSelector::onLoadBiomesClicked() {
        auto& repository = PluginContext::instance().biomeRepository();
        BiomeLoader loader(repository, this);
        loader.openFileDialog();
    }

    // Редактор: создался, заблокировал интерфейс (модально), закрылся, удалился
    void BiomeSelector::onEditBiomesClicked() {
        auto& repository = PluginContext::instance().biomeRepository();
        BiomeEditor editor(repository, this);
        editor.exec();
    }

    void BiomeSelector::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
        if (!m_resourcesLoaded) {
            Cajander::Gui::Utils::setupThemeIcon(m_loadButton, QString("/mActionFileOpen.svg"), QString("BiomeSelector"));
            Cajander::Gui::Utils::setupThemeIcon(m_editButton, QString("/mActionToggleEditing.svg"), QString("BiomeSelector"));
            m_resourcesLoaded = true;
        }
    }
}
