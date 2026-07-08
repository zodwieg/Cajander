cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)

# Собираем файлы репозитория из подпапки biomes
set(SERVICE_SOURCES
    biomes/biomeRepository.cpp
)

set(SERVICE_HEADERS 
    biomes/biomeRepository.h
    biomes/ibiomestorage.h
)

# Создаем статическую библиотеку слоёв сервисов
add_library(cajander_services STATIC ${SERVICE_SOURCES} ${SERVICE_HEADERS})

target_include_directories(cajander_services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Линкуем зависимости слоя сервисов
target_link_libraries(cajander_services 
    PUBLIC 
        cajander_domain  # Наш доменный слой с Biome struct
        Qt5::Core        # Нужен для QObject::tr и строк
)
