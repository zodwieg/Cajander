#pragma once
#include <QDialog>

#include "services/biomes/biomeRepository.h"

class QTabWidget;

class MainDialog : public QDialog {
    Q_OBJECT
public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog() override = default;
    void switchToTab(int index);
    void openProcessingTab();

private:
    QTabWidget* m_tabWidget = nullptr;
    Cajander::Services::BiomeRepository m_biomeRepository;
};
