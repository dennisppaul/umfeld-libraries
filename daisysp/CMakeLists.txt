cmake_minimum_required(VERSION 3.12)

project(umfeld-library-daisysp)

set(DAISYSP_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(DAISYSP_PATH_REPOSITORY "${DAISYSP_PATH}/libraries/daisysp")

add_subdirectory(${DAISYSP_PATH_REPOSITORY} EXCLUDE_FROM_ALL)
add_library(umfeld::daisysp ALIAS DaisySP)

add_library(umfeld-library-daisysp INTERFACE)

target_link_libraries(umfeld-library-daisysp INTERFACE
        DaisySP
        umfeld-lib-interface
)

target_sources(umfeld-library-daisysp INTERFACE
        ${DAISYSP_PATH}/src/UmfeldDaisySP.cpp
)

# link libraries

target_include_directories(umfeld-library-daisysp INTERFACE
        "${DAISYSP_PATH}/src"
)
