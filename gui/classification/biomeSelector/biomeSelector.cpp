#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/plugincontext.h"
#include "gui/classification/biomeSelector/biomeModel.h"
#include "gui/utils/uiUtils.h"

#include "gui/classification/biomeLoader/biomeLoader.h"
#include "gui/classification/biomeEditor/biomeEditor.h"
#include "services/biomes/biomeRepository.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QStyledItemDelegate>

namespace Cajander::Gui {

    BiomeSelector::BiomeSelector(QWidget* parent)
        : QWidget(parent)
    {
        m_model = &PluginContext::instance().biomeModel(); 
        auto& repository = PluginContext::instance().biomeRepository();

        auto* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 4, 0, 4); 
        mainLayout->setSpacing(6);

        m_schemeNameLabel = new QLabel(this);
        m_schemeNameLabel->setStyleSheet("color: #555555; font-size: 11px; font-weight: bold;");
        mainLayout->addWidget(m_schemeNameLabel);

        auto* controlsLayout = new QHBoxLayout();
        controlsLayout->setContentsMargins(0, 0, 0, 0);
        controlsLayout->setSpacing(6); 

        auto* labelSelectBiome = new QLabel(tr("Select Biome:"), this);
        controlsLayout->addWidget(labelSelectBiome, 0);

        m_comboBox = new QComboBox(this);

        m_loadButton = new QPushButton(this);
        m_loadButton->setToolTip(tr("Load Biomes JSON"));
        m_loadButton->setFixedSize(30, 24);

        m_editButton = new QPushButton(this);
        m_editButton->setToolTip(tr("Edit Biomes"));
        m_editButton->setFixedSize(30, 24);

        if (m_model) {
            m_comboBox->setModel(m_model);
            onComboBoxIndexChanged(m_comboBox->currentIndex());
        }

        controlsLayout->addWidget(m_comboBox, 1); 
        controlsLayout->addWidget(m_loadButton, 0); 
        controlsLayout->addWidget(m_editButton, 0);

        mainLayout->addLayout(controlsLayout);

        connect(m_comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &BiomeSelector::onComboBoxIndexChanged);
        connect(m_loadButton, &QPushButton::clicked, this, &BiomeSelector::onLoadBiomesClicked);
        connect(m_editButton, &QPushButton::clicked, this, &BiomeSelector::onEditBiomesClicked);

        connect(&repository, &Services::BiomeRepository::biomesChanged, this, &BiomeSelector::updateSchemeLabel);

        updateSchemeLabel();
    }

    void BiomeSelector::updateSchemeLabel() {
        auto& repository = PluginContext::instance().biomeRepository();
        QString schemeName = repository.getSchemeName();
        m_schemeNameLabel->setText(tr("Scheme: %1").arg(schemeName));
    }

    void BiomeSelector::onComboBoxIndexChanged(int index) {
        if (!m_model || index < 0) return;

        QModelIndex modelIndex = m_model->index(index, 0);
        if (modelIndex.isValid()) {
            QIcon icon = m_model->data(modelIndex, Qt::DecorationRole).value<QIcon>();
            m_comboBox->setItemIcon(index, icon); 

            emit biomeChanged(modelIndex);
        }
    }

    void BiomeSelector::onLoadBiomesClicked() {
        auto& repository = PluginContext::instance().biomeRepository();
        BiomeLoader loader(repository, this);
        loader.openFileDialog();
    }

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
