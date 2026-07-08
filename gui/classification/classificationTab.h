#pragma once

#include <QWidget>
#include <memory>

// Вперед, к строгому C++: прячем детали реализации в опережающие объявления
namespace Cajander::Services { class BiomeRepository; }
namespace Cajander::Gui { class BiomeModel; class BiomeSelector; }

class ClassificationTab : public QWidget {
    Q_OBJECT
public:
    // Внедряем зависимость: теперь вкладка требует ссылку на репозиторий для инициализации
    explicit ClassificationTab(const Cajander::Services::BiomeRepository& repository, QWidget* parent = nullptr);
    ~ClassificationTab() override = default;

private slots:
    // Слот для обработки выбора биома пользователем
    void onBiomeChanged(const QModelIndex& index);
    
    // Слот для будущей кнопки "карандашика"
    void onEditBiomesRequested();

private:
    // Умный указатель std::unique_ptr управляет временем жизни модели.
    // Модель удалится автоматически вместе с уничтожением вкладки — никаких утечек памяти!
    std::unique_ptr<Cajander::Gui::BiomeModel> m_biomeModel;
    
    // Указатель на наш кастомный виджет (Qt сам удалит его, так как он будет добавлен в Layout)
    Cajander::Gui::BiomeSelector* m_biomeSelector{nullptr};
};
