set(DOMAIN_SOURCES
    # src/IndexCalculator.cpp # Ваши будущие файлы
)

set(DOMAIN_HEADERS
    biome.h
)

add_library(cajander_domain STATIC ${DOMAIN_SOURCES} ${DOMAIN_HEADERS})

target_include_directories(cajander_domain 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE ${GDAL_INCLUDE_DIRS} # Здесь будет GDAL
)

# Линкуем только то, что нужно для расчетов
target_link_libraries(cajander_domain 
    PRIVATE ${GDAL_LIBRARIES}
)
