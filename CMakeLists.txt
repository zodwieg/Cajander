cmake_minimum_required(VERSION 3.16)
project(cajander_plugin VERSION 0.2.1 LANGUAGES CXX)

set(ENV{PATH} "C:/OSGeo4W/bin;C:/OSGeo4W/apps/Qt5/bin;C:/OSGeo4W/apps/qgis/bin;$ENV{PATH}")

# Настройки C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматический вызов MOC (обязательно для Q_OBJECT)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Поиск зависимостей (Qt5 и QGIS)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

# Пути к QGIS (подставьте ваши актуальные пути, если они отличаются)
if(NOT QGIS_INCLUDE_DIR)
    set(QGIS_INCLUDE_DIR "C:/OSGeo4W/apps/qgis-ltr-dev/include" CACHE PATH "Path to QGIS include dir")
endif()
if(NOT QGIS_LIBRARY)
    set(QGIS_LIBRARY "C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_core.lib;C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_gui.lib" CACHE STRING "Path to QGIS libs")
endif()

# Сбор исходников
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)
file(GLOB_RECURSE HEADERS 
    "include/*.h"
)
message(STATUS "--- DEBUG: QGIS INCLUDE DIR IS: ${QGIS_INCLUDE_DIR} ---")
# Создание динамической библиотеки (плагина)
add_library(${PROJECT_NAME} MODULE ${SOURCES} ${HEADERS})

target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

# Настройка путей поиска инклудов
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${QGIS_INCLUDE_DIR}
    ${QGIS_INCLUDE_DIR}/core
    ${QGIS_INCLUDE_DIR}/gui
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
)

# Линовка библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QGIS_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
)

# --- АВТОМАТИЧЕСКИЙ ДЕПЛОЙ ---
# Путь к профилю QGIS (по умолчанию для Windows)
set(QGIS_PLUGIN_DIR "$ENV{APPDATA}/QGIS/QGIS3/profiles/default/python/plugins/cajander")

# Создаем команду для копирования DLL и управляющих файлов после сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${QGIS_PLUGIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${QGIS_PLUGIN_DIR}/cajander.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_SOURCE_DIR}/metadata.txt" "${QGIS_PLUGIN_DIR}/metadata.txt"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_SOURCE_DIR}/__init__.py" "${QGIS_PLUGIN_DIR}/__init__.py"
    COMMENT "Deploying complete plugin package (DLL + Meta + Python Bridge) to QGIS..."
)
