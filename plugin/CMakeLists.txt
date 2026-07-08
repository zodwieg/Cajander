# plugin/CMakeLists.txt

set(PLUGIN_SOURCES
    plugin.cpp # Бывший main.cpp
)

# Создаем плагин как MODULE (динамическая библиотека .dll)
add_library(cajander MODULE ${PLUGIN_SOURCES})

# Настраиваем инклуды для точки входа плагина
target_include_directories(cajander PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${QGIS_INCLUDE_DIR}
    ${QGIS_INCLUDE_DIR}/core
    ${QGIS_INCLUDE_DIR}/gui
)

# Линкуем всё воедино. Магия в том, что cajander_gui сам притянет за собой cajander_core и Qt5::Widgets!
target_link_libraries(cajander PRIVATE
    cajander_gui
    cajander_services
    ${QGIS_LIBRARIES}
    Qt5::Xml
)

# --- AUTO DEPLOY (Windows) ---
set(QGIS_PLUGIN_DIR "$ENV{APPDATA}/QGIS/QGIS3/profiles/default/python/plugins/cajander")
file(TO_NATIVE_PATH "${QGIS_PLUGIN_DIR}" QGIS_PLUGIN_DIR)

# На дворе 2026 год, а генератор post-build команд в CMake все так же надежен :)
add_custom_command(TARGET cajander POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${QGIS_PLUGIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:cajander>" "${QGIS_PLUGIN_DIR}/cajander.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../metadata.txt" "${QGIS_PLUGIN_DIR}/metadata.txt"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../__init__.py" "${QGIS_PLUGIN_DIR}/__init__.py"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../default_biomes.json" "${QGIS_PLUGIN_DIR}/default_biomes.json"
    COMMENT "Deploying complete plugin package (DLL + Meta + Python Bridge) to QGIS..."
)
