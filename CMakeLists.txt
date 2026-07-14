cmake_minimum_required(VERSION 3.16)
project(cajander_project VERSION 0.3.2 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "C:/OSGeo4W/apps/Qt5")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_definitions(_USE_MATH_DEFINES)

find_package(Qt5 COMPONENTS Core Gui Widgets Xml REQUIRED)

if(NOT QGIS_INCLUDE_DIR)
    set(QGIS_INCLUDE_DIR "C:/OSGeo4W/apps/qgis-ltr-dev/include" CACHE PATH "Path to QGIS include dir")
endif()

if(NOT QGIS_LIBRARIES)
    set(QGIS_LIBRARIES 
        "C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_core.lib"
        "C:/OSGeo4W/apps/qgis-ltr-dev/lib/qgis_gui.lib" 
        CACHE STRING "Path to QGIS libs"
    )
endif()

add_subdirectory(domain)          # 1. Domain (depends on nothing)
add_subdirectory(services)        # 2. Business logic (depends on domain)
add_subdirectory(infrastructure)  # 3. Context and storage (depends on domain and services)
add_subdirectory(gui)             # 4. Interface (depends on all previois layers)
add_subdirectory(plugin)          # 5. Main plugin module (builds dll)
