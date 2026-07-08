#pragma once
#include <QWidget>

class ProcessingTab : public QWidget {
    Q_OBJECT
public:
    explicit ProcessingTab(QWidget* parent = nullptr);
    ~ProcessingTab() override = default;
};
