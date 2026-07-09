#include "plugincontext.h"
#include "services/biomes/biomerepository.h"
#include "infrastructure/biomes/jsonbiomestorage.h"
#include "gui/classification/biomeSelector/biomeModel.h"

namespace Cajander {

PluginContext& PluginContext::instance() {
    static PluginContext self;
    return self;
}

PluginContext::PluginContext() {
    auto jsonStorage = std::make_unique<Infrastructure::JsonBiomeStorage>();
    m_biomeRepository = std::make_unique<Services::BiomeRepository>(std::move(jsonStorage));
    
    m_biomeRepository->loadFromStorage();

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
