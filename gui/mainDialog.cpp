#include "gui/MainDialog.h"
#include "gui/data/DataTab.h"
#include "gui/classification/ClassificationTab.h"
#include "gui/processing/ProcessingTab.h"
#include "gui/help/HelpTab.h"

#include <QVBoxLayout>
#include <QTabWidget>

MainDialog::MainDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Cajander Biome Classifier");
    resize(500, 400);
    auto* mainLayout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget(this);

    m_tabWidget->addTab(new DataTab(this), "Data");
    m_tabWidget->addTab(new ClassificationTab(m_biomeRepository, this), tr("Classification"));
    m_tabWidget->addTab(new ProcessingTab(this), "Processing");
    m_tabWidget->addTab(new HelpTab(this), "Help");

    mainLayout->addWidget(m_tabWidget);
    switchToTab(1);
}

void MainDialog::switchToTab(int index) {
    if (m_tabWidget && index >= 0 && index < m_tabWidget->count()) {
        m_tabWidget->setCurrentIndex(index);
    }
}

void MainDialog::openProcessingTab() {
    switchToTab(2);
}
