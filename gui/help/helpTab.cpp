#include "gui/help/HelpTab.h"
#include <QVBoxLayout>
#include <QLabel>

HelpTab::HelpTab(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Help Tab Content Placeholder", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
