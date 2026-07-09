#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/plugincontext.h"
#include "gui/classification/biomeSelector/biomeModel.h"
#include "gui/utils/uiUtils.h"
#include "biomeLoader.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QStyle>

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
            m_loader = std::make_unique<BiomeLoader>(m_model, this);
        }

        layout->addWidget(m_comboBox, 1); 
        layout->addWidget(m_loadButton, 0); 
        layout->addWidget(m_editButton, 0);

        connect(m_comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &BiomeSelector::onComboBoxIndexChanged);
        if (m_loader) {
            connect(m_loadButton, &QPushButton::clicked, m_loader.get(), &BiomeLoader::openFileDialog);
        }
        connect(m_editButton, &QPushButton::clicked, this, &BiomeSelector::editBiomesRequested);
    }

    void BiomeSelector::onComboBoxIndexChanged(int index) {
        if (!m_model || index < 0) return;

        QModelIndex modelIndex = m_model->index(index, 0);
        if (modelIndex.isValid()) {
            emit biomeChanged(modelIndex);
        }
    }

    void BiomeSelector::showEvent(QShowEvent* event) 
    {
        QWidget::showEvent(event);

        if (!m_resourcesLoaded) {
            Cajander::Gui::Utils::setupThemeIcon(m_loadButton, QString("/mActionFileOpen.svg"), QString("BiomeSelector"));
            Cajander::Gui::Utils::setupThemeIcon(m_editButton, QString("/mActionToggleEditing.svg"), QString("BiomeSelector"));
            m_resourcesLoaded = true;
        }
    }
}