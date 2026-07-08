# core/CMakeLists.txt
set(CORE_SOURCES
    # src/IndexCalculator.cpp # Ваши будущие файлы
)

set(CORE_HEADERS
    # include/IndexCalculator.h
)

# Делаем STATIC или OBJECT библиотеку
add_library(cajander_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Указываем, где искать заголовки core
target_include_directories(cajander_core 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${GDAL_INCLUDE_DIRS} # Здесь будет GDAL
)

# Линкуем только то, что нужно для расчетов
target_link_libraries(cajander_core 
    PRIVATE ${GDAL_LIBRARIES}
)
