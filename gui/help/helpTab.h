#pragma once
#include <QWidget>

class HelpTab : public QWidget {
    Q_OBJECT
public:
    explicit HelpTab(QWidget* parent = nullptr);
    ~HelpTab() override = default;
};
