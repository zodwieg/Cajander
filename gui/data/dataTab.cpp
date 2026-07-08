#include "gui/data/DataTab.h"
#include <QVBoxLayout>
#include <QLabel>

DataTab::DataTab(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Data Tab Content Placeholder", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}
