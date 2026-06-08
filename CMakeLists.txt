cmake_minimum_required(VERSION 3.16)

project(Qt6-Paint VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

qt_standard_project_setup()

set(SRC_FILES
    src/main.cpp
    src/core/history_manager.cpp
    src/core/paint_manager.cpp
    src/core/settings.cpp
    src/core/viewport.cpp
    src/widgets/canvas.cpp
    src/widgets/layer_menu.cpp
    src/widgets/main_window.cpp
    src/widgets/ribbon_lower.cpp
    src/widgets/ribbon_upper.cpp
)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_SOURCE_DIR}/include/core/*.hpp
    ${CMAKE_SOURCE_DIR}/include/widgets/*.hpp
)

qt_add_resources(RESOURCES_FILES assets/resources.qrc)

qt_add_executable(Qt6-Paint
    ${SRC_FILES}
    ${HEADER_FILES}
    ${RESOURCES_FILES}
)

target_include_directories(Qt6-Paint PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(Qt6-Paint PRIVATE Qt6::Widgets Qt6::Core)