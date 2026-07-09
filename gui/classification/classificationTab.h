#pragma once

#include <QWidget>
#include <memory>

namespace Cajander::Gui { class BiomeSelector; }

class ClassificationTab : public QWidget {
    Q_OBJECT
public:
    explicit ClassificationTab(QWidget* parent = nullptr);
    ~ClassificationTab() override = default;

private slots:
    void onBiomeChanged(const QModelIndex& index);

private:
    Cajander::Gui::BiomeSelector* m_biomeSelector{nullptr};
};
