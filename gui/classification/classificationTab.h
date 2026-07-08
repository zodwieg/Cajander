#pragma once
#include <QWidget>

class ClassificationTab : public QWidget {
    Q_OBJECT
public:
    explicit ClassificationTab(QWidget* parent = nullptr);
    ~ClassificationTab() override = default;
};
