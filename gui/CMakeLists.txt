# Включаем автоматическую генерацию MOC-файлов для Qt макросов (Q_OBJECT)
# Без этого линкер выдаст ошибку vtable
set(CMAKE_AUTOMOC ON)

set(GUI_SOURCES
    MainDialog.cpp
    data/DataTab.cpp
    classification/ClassificationTab.cpp
    classification/biomeSelector/biomeSelector.cpp
    classification/biomeSelector/biomeModel.cpp
    processing/ProcessingTab.cpp
    help/HelpTab.cpp
)

set(GUI_HEADERS
    MainDialog.h
    data/DataTab.h
    classification/ClassificationTab.h
    classification/biomeSelector/biomeSelector.h
    classification/biomeSelector/biomeModel.h
    processing/ProcessingTab.h
    help/HelpTab.h
)

# 1. Меняем OBJECT на STATIC. Это создаст полноценный .lib / .a архив.
add_library(cajander_gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})

target_include_directories(cajander_gui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${QGIS_INCLUDE_DIR}
)

# 3. Связываем GUI с компонентами Qt.
# Меняем PRIVATE на PUBLIC, чтобы слой plugin автоматически наследовал зависимости от Qt
target_link_libraries(cajander_gui PUBLIC
    cajander_domain
    cajander_services
    cajander_infrastructure
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${QGIS_LIBRARIES}
)
