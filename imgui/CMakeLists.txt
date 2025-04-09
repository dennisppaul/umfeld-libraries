cmake_minimum_required(VERSION 3.12)

project(umfeld-library-imgui)

set(IMGUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(IMGUI_PATH_REPOSITORY "${IMGUI_PATH}/libraries/imgui")

add_library(umfeld-library-imgui INTERFACE)

target_sources(umfeld-library-imgui INTERFACE
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

target_link_libraries(umfeld-library-imgui INTERFACE umfeld-lib-interface)
target_include_directories(umfeld-library-imgui INTERFACE
        "${IMGUI_PATH}/include"
        "${IMGUI_PATH_REPOSITORY}"
        "${IMGUI_PATH_REPOSITORY}/backends"
)
