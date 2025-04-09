cmake_minimum_required(VERSION 3.12)

project(umfeld-library-opencv)

## OpenCV

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENCV REQUIRED opencv4)
message(STATUS "OpenCV Include Dirs: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "OpenCV Libraries: ${OPENCV_LIBRARIES}")

## library

file(GLOB UMFELD_OPENCV_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(umfeld-library-opencv STATIC ${UMFELD_OPENCV_SOURCE_FILES})
# add_library(umfeld-library-opencv INTERFACE ${UMFELD_OPENCV_SOURCE_FILES}) # use `INTERFACE` if *header-only* library

target_include_directories(umfeld-library-opencv PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)

target_link_libraries(umfeld-library-opencv PUBLIC ${OPENCV_LIBRARIES})
