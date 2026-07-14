/**
 * @file plugin.cpp
 * @brief Core entry points and lifecycle management for the CAJANDER QGIS C++ plugin.
 * @details Implements the mandatory QGIS C-API functions required for plugin 
 * registration, UI initialization, and cleanup.
 */

#include <qgisinterface.h>
#include <qgsproject.h>
#include <QAction> 
#include <QIcon>
#include "gui/MainDialog.h"
#include "infrastructure/pluginContext.h"

/**
 * @name Global State Pointers
 * @{
 */
/// Pointer to the QGIS application interface wrapper.
QgisInterface* g_qgisInterface = nullptr;

/// Action added to the QGIS toolbar and menu to launch the plugin.
QAction* g_pluginAction = nullptr;

/// Main dialog window instance of the CAJANDER plugin.
Cajander::Gui::MainDialog* g_mainDialog = nullptr;
/** @} */

#if defined(_MSC_VER)
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

    /**
     * @brief Returns the display name of the plugin.
     * @return A static C-string containing the plugin title.
     */
    PLUGIN_EXPORT const char* name() {
        return "CAJANDER Biome Classification";
    }

    /**
     * @brief Returns a short description of the plugin's purpose.
     * @return A static C-string containing the description.
     */
    PLUGIN_EXPORT const char* description() {
        return "C++ plugin for biome indexing and classification using GDAL.";
    }

    /**
     * @brief Returns the current semantic version of the plugin.
     * @return A static C-string containing the version number (e.g., "0.3.5").
     */
    PLUGIN_EXPORT const char* version() {
        return "0.3.5";
    }

    /**
     * @brief Returns the plugin type category for QGIS.
     * @return An integer where 1 typically represents a standard UI plugin.
     */
    PLUGIN_EXPORT int type() {
        return 1;
    }

    /**
     * @brief Initializes the plugin, creates UI elements, and registers them within QGIS.
     * @details Bootstraps the application context, configures the QAction triggers, 
     * and injects icons into the QGIS Raster toolbar and menu.
     * @param iface Pointer to the host QGIS interface object.
     */
    PLUGIN_EXPORT void initPlugin(QgisInterface* iface) {
        g_qgisInterface = iface;

        // Initialize the global application context (Singleton)
        Cajander::PluginContext::instance();

        g_pluginAction = new QAction(QObject::tr("Run CAJANDER"), g_qgisInterface->mainWindow());
        g_pluginAction->setIcon(QIcon(":/plugins/cajander/icon.png"));

        QObject::connect(g_pluginAction, &QAction::triggered, []() {
            if (!g_mainDialog) {
                g_mainDialog = new Cajander::Gui::MainDialog(g_qgisInterface->mainWindow());
                g_mainDialog->setAttribute(Qt::WA_DeleteOnClose);
                
                QObject::connect(g_mainDialog, &QObject::destroyed, []() {
                    g_mainDialog = nullptr;
                });
            }
            g_mainDialog->show();
            g_mainDialog->raise();
            g_mainDialog->activateWindow();
        });

        g_qgisInterface->addRasterToolBarIcon(g_pluginAction);
        g_qgisInterface->addPluginToRasterMenu("&CAJANDER", g_pluginAction);
    }

    /**
     * @brief Unloads the plugin, destroys UI components, and cleans up references.
     * @details Safely closes open dialogs, removes toolbars/menus from QGIS interface, 
     * and deallocates allocated raw pointers to prevent memory leaks.
     */
    PLUGIN_EXPORT void unloadPlugin() {
        if (g_mainDialog) {
            g_mainDialog->close(); 
        }

        if (g_qgisInterface && g_pluginAction) {
            g_qgisInterface->removePluginRasterMenu("&CAJANDER", g_pluginAction);
            g_qgisInterface->removeRasterToolBarIcon(g_pluginAction);
            delete g_pluginAction;
            g_pluginAction = nullptr;
        }
    }

} // extern "C"
