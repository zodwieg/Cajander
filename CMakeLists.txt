cmake_minimum_required(VERSION 3.16)
project(cajander_project VERSION 0.3.2 LANGUAGES CXX) # Переименовал проект, чтобы не путать с бинарником

# --- НАСТРОЙКА ПУТЕЙ OSGeo4W (Windows) ---
# Подсказываем CMake, где искать Qt5Config.cmake и другие пакеты.
# Это гарантирует, что find_package(Qt5) отработает без ошибок.
list(APPEND CMAKE_PREFIX_PATH "C:/OSGeo4W/apps/Qt5")

# Настройки стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Глобально включаем генераторы Qt (чтобы не дублировать в gui и plugin)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Глобальные дефайны
add_compile_definitions(_USE_MATH_DEFINES)

# --- ПОИСК ЗАВИСИМОСТЕЙ ---
find_package(Qt5 COMPONENTS Core Gui Widgets Xml REQUIRED)

# Хардкод путей QGIS (перекочевал из вашего конфига, пока оставляем так)
if(NOT QGIS_INCLUDE_DIR)
    set(QGIS_INCLUDE_DIR "C:/OSGeo4W/apps/qgis-ltr-dev/include" CACHE PATH "Path to QGIS include dir")
endif()

# Отредактировал под линковку: в CMake лучше разделять библиотеки списком через точку с запятой
if(NOT QGIS_LIBRARIES)
    set(QGIS_LIBRARIES 
        "C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_core.lib"
        "C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_gui.lib" 
        CACHE STRING "Path to QGIS libs"
    )
endif()

# --- ПОДКЛЮЧЕНИЕ СЛОЕВ (Порядок важен!) ---
add_subdirectory(domain)          # 1. Чистый домен (не зависит ни от чего)
add_subdirectory(services)        # 2. Бизнес-логика (зависит только от domain)
add_subdirectory(infrastructure)  # 3. Контекст и хранилище (зависит от domain и services)
add_subdirectory(gui)             # 4. Интерфейс (зависит от всех предыдущих, так как видит модели и контекст)
add_subdirectory(plugin)          # 5. Главный модуль плагина (собирает dll)
