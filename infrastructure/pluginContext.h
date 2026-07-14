#pragma once

#include <memory>

namespace Cajander::Services { class BiomeRepository; }
namespace Cajander::Gui      { class BiomeModel; }

namespace Cajander {

class PluginContext {
public:
    static PluginContext& instance();

    PluginContext(const PluginContext&) = delete;
    PluginContext& operator=(const PluginContext&) = delete;
    PluginContext(PluginContext&&) = delete;
    PluginContext& operator=(PluginContext&&) = delete;

    Cajander::Services::BiomeRepository& biomeRepository() const;
    Cajander::Gui::BiomeModel& biomeModel() const;

private:
    PluginContext();
    ~PluginContext();

    std::unique_ptr<Cajander::Services::BiomeRepository> m_biomeRepository;
    std::unique_ptr<Cajander::Gui::BiomeModel> m_biomeModel;
};

} // namespace Cajander
