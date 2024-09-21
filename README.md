# umgebung / libraries

@TODO(add a *How to add Libraries* text)

```CMake
cmake_minimum_required(VERSION 3.12)

project(umgebung-library-opencv)

## OpenCV

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENCV REQUIRED opencv4)
message(STATUS "OpenCV Include Dirs: ${OPENCV_INCLUDE_DIRS}")
message(STATUS "OpenCV Libraries: ${OPENCV_LIBRARIES}")

## library

file(GLOB UMGEBUNG_OPENCV_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(umgebung-library-opencv STATIC ${UMGEBUNG_OPENCV_SOURCE_FILES})
# add_library(umgebung-library-opencv INTERFACE ${UMGEBUNG_OPENCV_SOURCE_FILES}) # use `INTERFACE` if *header-only* library

target_include_directories(umgebung-library-opencv PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENCV_INCLUDE_DIRS}
)

target_link_libraries(umgebung-library-opencv PUBLIC ${OPENCV_LIBRARIES})
```

## proposed directory structure

```
.
├── CMakeLists.txt
├── README.md
├── examples
├── include
└── src
```