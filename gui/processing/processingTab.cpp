#include "gui/processing/ProcessingTab.h"
#include <QVBoxLayout>
#include <QLabel>

ProcessingTab::ProcessingTab(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Processing Tab Content Placeholder", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
