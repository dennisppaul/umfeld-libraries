cmake_minimum_required(VERSION 3.12)

project(umfeld-library-opencv)

set(OPENCV_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(umfeld-library-opencv INTERFACE)

target_sources(umfeld-library-opencv INTERFACE
        ${OPENCV_PATH}/src/UmfeldOpenCV.cpp
)

# add opencv

find_package(OpenCV REQUIRED)
target_link_libraries(umfeld-library-opencv INTERFACE
        ${OpenCV_LIBS}
        umfeld-lib-interface
)
#find_package(PkgConfig REQUIRED)
#pkg_search_module(OPENCV REQUIRED opencv4)
#message(STATUS "OpenCV Include Dirs: ${OPENCV_INCLUDE_DIRS}")
#message(STATUS "OpenCV Libraries: ${OPENCV_LIBRARIES}")
#target_link_libraries(umfeld-library-opencv INTERFACE
#        ${OPENCV_LIBRARIES}
#        umfeld-lib-interface
#)

# link libraries

target_link_libraries(umfeld-library-opencv INTERFACE umfeld-lib-interface)
target_include_directories(umfeld-library-opencv INTERFACE
        "${OPENCV_PATH}/src"
)
#target_include_directories(umfeld-library-opencv INTERFACE
#        "${OPENCV_PATH}/src"
#        "${OPENCV_INCLUDE_DIRS}"
#)

# embed Info.plist if on macOS to allow camera access

if (APPLE)
    # Specify the path to your Info.plist file
    set(INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

    # Ensure the Info.plist file exists
    if (EXISTS "${INFO_PLIST}")
        message(STATUS "adding Info.plist at ${INFO_PLIST}")
        target_link_options(${PROJECT_NAME} INTERFACE "SHELL:-sectcreate __TEXT __info_plist ${INFO_PLIST}")
    else ()
        message(FATAL_ERROR "Info.plist not found at ${INFO_PLIST}")
    endif ()
endif ()