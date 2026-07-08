#include "plugincontext.h"
#include "services/biomes/biomerepository.h"
#include "infrastructure/biomes/jsonbiomestorage.h"
#include "gui/classification/biomeSelector/biomeModel.h"

namespace Cajander {

PluginContext& PluginContext::instance() {
    // Гарантирует создание в единственном экземпляре при первом вызове
    // и автоматическое уничтожение при выгрузке плагина/закрытии QGIS
    static PluginContext self;
    return self;
}

PluginContext::PluginContext() {
    // 1. Инициализируем инфраструктуру данных
    auto jsonStorage = std::make_unique<Infrastructure::JsonBiomeStorage>();
    m_biomeRepository = std::make_unique<Services::BiomeRepository>(std::move(jsonStorage));
    
    // Сразу читаем JSON с диска в память
    m_biomeRepository->loadFromStorage();

    // 2. Инициализируем UI-модели (Синглтоны)
    // Передаем репозиторий внутрь модели. Родителем (nullptr) управлять не нужно,
    // так как m_biomeModel завернут в std::unique_ptr и уничтожится вместе с контекстом.
    m_biomeModel = std::make_unique<Gui::BiomeModel>(*m_biomeRepository);
}

PluginContext::~PluginContext() = default;

Cajander::Services::BiomeRepository& PluginContext::biomeRepository() const {
    return *m_biomeRepository;
}

Cajander::Gui::BiomeModel& PluginContext::biomeModel() const {
    return *m_biomeModel;
}

} // namespace Cajander
