#include <qgisinterface.h>
#include <qgsproject.h>
#include <QAction> 
#include <QIcon>
#include "gui/MainDialog.h"
#include "infrastructure/pluginContext.h"

QgisInterface* g_qgisInterface = nullptr;
QAction* g_pluginAction = nullptr;
MainDialog* g_mainDialog = nullptr; 

#if defined(_MSC_VER)
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

    PLUGIN_EXPORT const char* name() {
        return "CAJANDER Biome Classification";
    }

    PLUGIN_EXPORT const char* description() {
        return "C++ plugin for biome indexing and classification using GDAL.";
    }

    PLUGIN_EXPORT const char* version() {
        return "0.3.5";
    }

    PLUGIN_EXPORT int type() {
        return 1;
    }

    PLUGIN_EXPORT void initPlugin(QgisInterface* iface) {
        g_qgisInterface = iface;

        Cajander::PluginContext::instance();

        g_pluginAction = new QAction(QObject::tr("Run CAJANDER"), g_qgisInterface->mainWindow());
        g_pluginAction->setIcon(QIcon(":/plugins/cajander/icon.png"));

        QObject::connect(g_pluginAction, &QAction::triggered, []() {
            if (!g_mainDialog) {
                g_mainDialog = new MainDialog(g_qgisInterface->mainWindow());
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
