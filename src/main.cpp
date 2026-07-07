#include <qgisinterface.h> // Интерфейс QGIS API
#include <qgsproject.h>    // Доступ к текущему проекту QGIS
#include <QAction>         // Класс кнопки в меню/панели инструментов QGIS
#include "gui/MainDialog.h"

// Глобальные указатели (в рамках нашей DLL), которыми мы управляем.
// В отличие от C#, в C++ плагинах для QGIS эти указатели часто живут на верхнем уровне модуля.
QgisInterface* g_qgisInterface = nullptr;
QAction* g_pluginAction = nullptr;

// Макрос для Windows/Linux, который делает функции видимыми снаружи DLL
#if defined(_MSC_VER)
    #define PLUGIN_EXPORT __declspec(dllexport)
#else
    #define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

// Все функции взаимодействия с QGIS должны быть в блоке extern "C"
extern "C" {

    // 1. Обязательная функция: QGIS узнает имя и описание плагина
    PLUGIN_EXPORT const char* name() {
        return "CAJANDER Biome Classification";
    }

    PLUGIN_EXPORT const char* description() {
        return "C++ plugin for biome indexing and classification using GDAL.";
    }

    PLUGIN_EXPORT const char* version() {
        return "0.2.0";
    }

    PLUGIN_EXPORT int type() {
        // 1 означает стандартный плагин с пользовательским интерфейсом
        return 1; 
    }

    // 2. Инициализация плагина. Вызывается ОДИН РАЗ при старте QGIS или включении плагина.
    // Сюда QGIS передает указатель на самого себя (iface).
    PLUGIN_EXPORT void initPlugin(QgisInterface* iface) {
        g_qgisInterface = iface;

        // Создаем кнопку (Action), которая появится в меню QGIS
        g_pluginAction = new QAction(QObject::tr("Run CAJANDER"), g_qgisInterface->mainWindow());
        
        // Добавляем иконку (пока стандартную из QGIS или пустую)
        g_pluginAction->setIcon(QIcon(":/plugins/cajander/icon.png"));

        // Подключаем событие клика по кнопке к нашей функции запуска плагина.
        // Это аналог подписки на событие (click) в Angular!
        // Передаем: КТО инициирует, КАКОЙ сигнал, КТО обрабатывает, КАКОЙ метод вызывается.
        QObject::connect(g_pluginAction, &QAction::triggered, []() {
            // Создаем экземпляр нашего диалога. 
            // Передаем главное окно QGIS как родителя (g_qgisInterface->mainWindow())
            // Благодаря этому плагин не свернется отдельно от QGIS и правильно отцентрируется.
            MainDialog dialog(g_qgisInterface->mainWindow());
            
            // exec() открывает окно как МОДАЛЬНОЕ (блокирует QGIS, пока плагин открыт).
            // Это самый безопасный способ для ГИС-плагинов: пользователь не сможет 
            // случайно удалить слой с карты в QGIS, пока наш плагин считает по нему индексы.
            dialog.exec(); 
        });

        // Добавляем созданную кнопку в меню "Растр" -> "CAJANDER"
        g_qgisInterface->addRasterToolBarIcon(g_pluginAction);
        g_qgisInterface->addPluginToRasterMenu("&CAJANDER", g_pluginAction);
    }

    // 3. Выгрузка плагина. Вызывается при закрытии QGIS или отключении плагина в менеджере.
    // Мы ОБЯЗАНЫ убрать за собой всё, что добавили в интерфейс QGIS, иначе программа упадет.
    PLUGIN_EXPORT void unloadPlugin() {
        if (g_qgisInterface && g_pluginAction) {
            // Удаляем кнопку из меню и панели инструментов
            g_qgisInterface->removePluginRasterMenu("&CAJANDER", g_pluginAction);
            g_qgisInterface->removeRasterToolBarIcon(g_pluginAction);
            
            // В C++ мы должны сами удалить объект QAction из кучи (heap), 
            // так как мы его создавали через оператор `new`.
            delete g_pluginAction;
            g_pluginAction = nullptr;
        }
    }

} // extern "C"
