#include "gui/classification/ClassificationTab.h"
#include "gui/classification/biomeSelector/biomeSelector.h"
#include "infrastructure/pluginContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

namespace Cajander::Gui {

ClassificationTab::ClassificationTab(QWidget* parent) 
    : QWidget(parent) 
{
    // Configure top-level layout geometry with standard margins.
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // Initialize custom biome selector widget. Memory managed via Qt parent-child hierarchy.
    m_biomeSelector = new BiomeSelector(this);

    auto* formLayout = new QFormLayout();
    formLayout->addRow(m_biomeSelector);
    
    mainLayout->addLayout(formLayout);

    // Pushes all widgets up to prevent vertical stretching.
    mainLayout->addStretch(1);

    // Connect using member function pointers. No namespaces required inside the block.
    connect(m_biomeSelector, &BiomeSelector::biomeChanged, 
            this, &ClassificationTab::onBiomeChanged);
}

void ClassificationTab::onBiomeChanged(const QModelIndex& index) {
    // TODO: Forward the index updates to the model controller or validation layer.
}

} // namespace Cajander::Gui
