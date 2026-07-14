add_library(cajander_domain INTERFACE)

target_include_directories(cajander_domain 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(cajander_domain 
    INTERFACE 
        Qt5::Core
        Qt5::Gui
)