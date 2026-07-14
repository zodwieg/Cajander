#pragma once

#include <memory>

namespace Cajander::Services { class BiomeRepository; }
namespace Cajander::Gui      { class BiomeListModel; }

namespace Cajander {

/**
 * @brief Global Dependency Injection container and Application Context.
 * @details Implements a thread-safe Meyers' Singleton pattern. Manages the lifecycle 
 * of core services, repositories, and UI models, acting as the central composition root.
 */
class PluginContext {
public:
    /**
     * @brief Provides global access to the single instance of the application context.
     * @return Reference to the thread-safe static instance of PluginContext.
     */
    static PluginContext& instance();

    // Delete copy/move constructors and assignment operators to guarantee uniqueness
    PluginContext(const PluginContext&) = delete;
    PluginContext& operator=(const PluginContext&) = delete;
    PluginContext(PluginContext&&) = delete;
    PluginContext& operator=(PluginContext&&) = delete;

    /**
     * @brief Gets the reference to the active biome repository service.
     * @return Reference to the BiomeRepository instance.
     */
    Cajander::Services::BiomeRepository& biomeRepository() const;

    /**
     * @brief Gets the reference to the active biome UI model (Qt Item Model).
     * @return Reference to the BiomeModel instance.
     */
    Cajander::Gui::BiomeListModel& biomeModel() const;

private:
    /**
     * @brief Private constructor to instantiate internal components.
     * @details Initializes infrastructure storages, services, repositories, and UI models.
     */
    PluginContext();

    /**
     * @brief Private destructor to cleanly tear down the context.
     */
    ~PluginContext();

    std::unique_ptr<Cajander::Services::BiomeRepository> m_biomeRepository;
    std::unique_ptr<Cajander::Gui::BiomeListModel> m_biomeModel;
};

} // namespace Cajander
