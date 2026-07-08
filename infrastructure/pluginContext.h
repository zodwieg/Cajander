#pragma once

#include <memory>

// Опережающие объявления
namespace Cajander::Services { class BiomeRepository; }
namespace Cajander::Gui      { class BiomeModel; }

namespace Cajander {

class PluginContext {
public:
    // Глобальная точка доступа (наш аналог AppInjector.get())
    static PluginContext& instance();

    // Запрещаем копирование и перемещение контейнера
    PluginContext(const PluginContext&) = delete;
    PluginContext& operator=(const PluginContext&) = delete;
    PluginContext(PluginContext&&) = delete;
    PluginContext& operator=(PluginContext&&) = delete;

    // Геттеры для сервисов и моделей
    Cajander::Services::BiomeRepository& biomeRepository() const;
    Cajander::Gui::BiomeModel& biomeModel() const;

private:
    // Конструктор и деструктор теперь ПРИВАТНЫЕ
    PluginContext();
    ~PluginContext();

    // Умные указатели на синглтон-сервисы и синглтон-модели
    std::unique_ptr<Cajander::Services::BiomeRepository> m_biomeRepository;
    std::unique_ptr<Cajander::Gui::BiomeModel> m_biomeModel;
};

} // namespace Cajander
