cmake_minimum_required(VERSION 3.12)

project(umgebung-imgui)                                            # set application name
set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umgebung") # set path to umgebung library

# --------- no need to change anything below this line ------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# add imgui

set(IMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(IMGUI_PATH_REPOSITORY "${IMGUI_PATH}/libraries/imgui")

add_library(imgui STATIC
        ${IMGUI_PATH_REPOSITORY}/imgui.cpp
        ${IMGUI_PATH_REPOSITORY}/imgui_demo.cpp
        ${IMGUI_PATH_REPOSITORY}/imgui_draw.cpp
        ${IMGUI_PATH_REPOSITORY}/imgui_tables.cpp
        ${IMGUI_PATH_REPOSITORY}/imgui_widgets.cpp
        ${IMGUI_PATH_REPOSITORY}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_PATH_REPOSITORY}/backends/imgui_impl_opengl2.cpp
        ${IMGUI_PATH_REPOSITORY}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_PATH_REPOSITORY}/backends/imgui_impl_sdlrenderer3.cpp
)

# link libraries

target_link_libraries(imgui PRIVATE umgebung-lib)
target_include_directories(imgui PUBLIC
        "${IMGUI_PATH}/include"
        "${IMGUI_PATH_REPOSITORY}"
        "${IMGUI_PATH_REPOSITORY}/backends"
)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

