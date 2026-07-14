#include "plugincontext.h"
#include "services/biomes/biomerepository.h"
#include "infrastructure/biomes/jsonbiomestorage.h"
#include "gui/classification/biomeSelector/biomeListModel.h"

namespace Cajander {

PluginContext& PluginContext::instance() {
    // Thread-safe initialized on the first call since C++11 (Magic Statics)
    static PluginContext self;
    return self;
}

PluginContext::PluginContext() {
    // 1. Initialize low-level infrastructure storage
    auto jsonStorage = std::make_unique<Infrastructure::JsonBiomeStorage>();

    // 2. Inject storage into repository and pass ownership via std::move
    m_biomeRepository = std::make_unique<Services::BiomeRepository>(std::move(jsonStorage));
    
    // 3. Eagerly load persistent biome data into memory
    m_biomeRepository->loadFromStorage();

    // 4. Wrap repository inside the Qt presentation model
    m_biomeModel = std::make_unique<Gui::BiomeListModel>(*m_biomeRepository);
}

// Default destructor is required here in the .cpp file, 
// because m_biomeRepository and m_biomeModel use forward-declared classes.
PluginContext::~PluginContext() = default;

Cajander::Services::BiomeRepository& PluginContext::biomeRepository() const {
    return *m_biomeRepository;
}

Cajander::Gui::BiomeListModel& PluginContext::biomeModel() const {
    return *m_biomeModel;
}

} // namespace Cajander
