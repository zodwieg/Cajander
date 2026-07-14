#include "gui/MainDialog.h"
#include "gui/data/DataTab.h"
#include "gui/classification/ClassificationTab.h"
#include "gui/processing/ProcessingTab.h"
#include "gui/help/HelpTab.h"
#include "infrastructure/pluginContext.h"

#include <QVBoxLayout>
#include <QTabWidget>

namespace Cajander::Gui {

MainDialog::MainDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Cajander Biome Classifier"));
    resize(500, 400);

    // Qt Memory Management: Layouts and widgets initialized with 'this' 
    // are automatically reparented and freed via RAII.
    auto* mainLayout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget(this);

    // Constructing tabs. Note that ClassificationTab is now also inside Cajander::Gui.
    m_tabWidget->addTab(new DataTab(this), tr("Data"));
    m_tabWidget->addTab(new ClassificationTab(this), tr("Classification"));
    m_tabWidget->addTab(new ProcessingTab(this), tr("Processing"));
    m_tabWidget->addTab(new HelpTab(this), tr("Help"));

    mainLayout->addWidget(m_tabWidget);

    // Start with the classification tab active by default.
    switchToTab(static_cast<int>(TabIndex::Classification));
}

void MainDialog::switchToTab(int index) {
    // Technical safety boundary check against total tab count.
    if (m_tabWidget && index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

void MainDialog::openProcessingTab() {
    // Explicit static cast of the typed enum to raw integer index.
    switchToTab(static_cast<int>(TabIndex::Processing));
}

} // namespace Cajander::Gui
