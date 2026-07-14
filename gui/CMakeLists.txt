set(CMAKE_AUTOMOC ON)

set(GUI_SOURCES
    MainDialog.cpp
    data/DataTab.cpp
    classification/ClassificationTab.cpp
    classification/biomeSelector/biomeSelector.cpp
    classification/biomeSelector/biomeListModel.cpp
    classification/biomeLoader/biomeLoader.cpp
    classification/biomeEditor/biomeEditor.cpp
    classification/biomeEditor/biomeTableModel.cpp
    classification/biomeEditor/biomeItemDelegate.cpp
    classification/biomeExporter/biomeExporter.cpp
    processing/ProcessingTab.cpp
    help/HelpTab.cpp
)

set(GUI_HEADERS
    utils/uiUtils.h
    MainDialog.h
    data/DataTab.h
    classification/ClassificationTab.h
    classification/biomeSelector/biomeSelector.h
    classification/biomeSelector/biomeListModel.h
    classification/biomeLoader/biomeLoader.h
    classification/biomeEditor/biomeEditor.h
    classification/biomeEditor/biomeTableModel.h
    classification/biomeEditor/biomeItemDelegate.h
    classification/biomeExporter/biomeExporter.h
    processing/ProcessingTab.h
    help/HelpTab.h
)

add_library(cajander_gui STATIC ${GUI_SOURCES} ${GUI_HEADERS})

target_include_directories(cajander_gui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${QGIS_INCLUDE_DIR}
)

target_link_libraries(cajander_gui PUBLIC
    cajander_domain
    cajander_services
    cajander_infrastructure
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${QGIS_LIBRARIES}
)
