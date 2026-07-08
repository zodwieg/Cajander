set(CMAKE_AUTOMOC ON)

set(INFRASTRUCTURE_SOURCES 
    plugincontext.cpp 
    biomes/jsonBiomeStorage.cpp
)

set(INFRASTRUCTURE_HEADERS 
    pluginContext.h 
    biomes/jsonBiomeStorage.h
)

add_library(cajander_infrastructure STATIC ${INFRASTRUCTURE_SOURCES} ${INFRASTRUCTURE_HEADERS})

# Инфраструктура связывает Сервисы и UI-модели (так как контекст знает про BiomeModel)
target_link_libraries(cajander_infrastructure
    PUBLIC cajander_services
    PRIVATE 
        Qt5::Core
        Qt5::Widgets
        # Сюда передаются переменные путей QGIS, которые ты закоммитил в корневом CMake
        ${QGIS_LIBRARIES} 
)

target_include_directories(cajander_infrastructure PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE ${QGIS_INCLUDE_DIR}
)
