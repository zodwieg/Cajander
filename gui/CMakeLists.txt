# Включаем автоматическую генерацию MOC-файлов для Qt макросов (Q_OBJECT)
# Без этого линкер выдаст ошибку vtable
set(CMAKE_AUTOMOC ON)

set(GUI_SOURCES
    MainDialog.cpp
    data/DataTab.cpp
    classification/ClassificationTab.cpp
    processing/ProcessingTab.cpp
    help/HelpTab.cpp
)

set(GUI_HEADERS
    MainDialog.h
    data/DataTab.h
    classification/ClassificationTab.h
    processing/ProcessingTab.h
    help/HelpTab.h
)

# 1. Меняем OBJECT на STATIC. Это создаст полноценный .lib / .a архив.
add_library(cajander_gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})

# 2. Настраиваем инклуды правильно
target_include_directories(cajander_gui 
    # PUBLIC говорит: "Любой, кто линкует cajander_gui, тоже должен искать заголовки здесь"
    # Это позволит писать #include "gui/MainDialog.h" в слое plugin
    PUBLIC 
        ${CMAKE_SOURCE_DIR} 
    # PRIVATE говорит: "Эти пути нужны только для компиляции самого GUI"
    PRIVATE 
        ${QGIS_INCLUDE_DIR}
        ${QGIS_INCLUDE_DIR}/core
        ${QGIS_INCLUDE_DIR}/gui
)

# 3. Связываем GUI с компонентами Qt.
# Меняем PRIVATE на PUBLIC, чтобы слой plugin автоматически наследовал зависимости от Qt
target_link_libraries(cajander_gui PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
