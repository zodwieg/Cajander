#pragma once

#include <QWidget>
#include <memory>

// Вперед, к строгому C++: прячем детали реализации в опережающие объявления
namespace Cajander::Gui { class BiomeSelector; }

class ClassificationTab : public QWidget {
    Q_OBJECT
public:
    explicit ClassificationTab(QWidget* parent = nullptr);
    ~ClassificationTab() override = default;

private slots:
    // Слот для обработки выбора биома пользователем
    void onBiomeChanged(const QModelIndex& index);
    
    // Слот для будущей кнопки "карандашика"
    void onEditBiomesRequested();

private:
    // Указатель на наш кастомный виджет (Qt сам удалит его, так как он будет добавлен в Layout)
    Cajander::Gui::BiomeSelector* m_biomeSelector{nullptr};
};
