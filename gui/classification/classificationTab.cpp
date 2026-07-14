#include "gui/classification/ClassificationTab.h"
#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/pluginContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

ClassificationTab::ClassificationTab(QWidget* parent) 
    : QWidget(parent) 
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    m_biomeSelector = new Cajander::Gui::BiomeSelector(this);

    auto* formLayout = new QFormLayout();
    formLayout->addRow(m_biomeSelector);
    
    mainLayout->addLayout(formLayout);

    mainLayout->addStretch(1);

    connect(m_biomeSelector, &Cajander::Gui::BiomeSelector::biomeChanged, 
            this, &ClassificationTab::onBiomeChanged);
}

void ClassificationTab::onBiomeChanged(const QModelIndex& index) {
}

