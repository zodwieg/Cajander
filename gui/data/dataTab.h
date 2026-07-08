#pragma once
#include <QWidget>

class DataTab : public QWidget {
    Q_OBJECT
public:
    explicit DataTab(QWidget* parent = nullptr);
    ~DataTab() override = default;
};
