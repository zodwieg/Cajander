#pragma once
#include <QDialog>

class MainDialog : public QDialog {
    Q_OBJECT
public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog() override = default; // Хорошая практика использовать override
};
