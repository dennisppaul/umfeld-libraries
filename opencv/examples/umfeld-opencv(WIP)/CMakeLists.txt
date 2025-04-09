cmake_minimum_required(VERSION 3.12)

project(umfeld-opencv)                                                                   # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld")                         # set path to umfeld library e.g `set(UMFELD_PATH "<absolute/path/to/library>")`
set(OPENCV_UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld-libraries/opencv") # set path to umfeld-opencv library
link_directories("/usr/local/lib")                                                         # optional, can help to fix issues with Homebrew on macOS

# set compiler flags to C++17 ( minimum required by umfeld, needs to go before `add_executable` )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X")

# add source + header files from this directory

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(".")

# add umfeld

target_compile_definitions(${PROJECT_NAME} PRIVATE UMFELD_WINDOW_TITLE="${PROJECT_NAME}") # set window title
add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# add umfeld-opencv

add_subdirectory(${OPENCV_UMFELD_PATH}
        ${CMAKE_BINARY_DIR}/umfeld-opencv-${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-opencv)

# embed Info.plist if on macOS to allow camera access
if (APPLE)
    # Specify the path to your Info.plist file
    set(INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

    # Ensure the Info.plist file exists
    if (EXISTS "${INFO_PLIST}")
        target_link_options(${PROJECT_NAME} PRIVATE "SHELL:-sectcreate __TEXT __info_plist ${INFO_PLIST}")
    else ()
        message(FATAL_ERROR "Info.plist not found at ${INFO_PLIST}")
    endif ()
endif ()
