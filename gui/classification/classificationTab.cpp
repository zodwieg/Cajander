#include "gui/classification/ClassificationTab.h"
#include <QVBoxLayout>
#include <QLabel>

ClassificationTab::ClassificationTab(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Classification Tab Content Placeholder", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
