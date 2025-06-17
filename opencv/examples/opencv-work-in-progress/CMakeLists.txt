cmake_minimum_required(VERSION 3.12)

project(opencv-work-in-progress)                                  # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umfeld") # set path to umfeld library
set(UMFELD_OPENCV_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")  # set path to umfeld opencv library

# --------- no need to change anything below this line ------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# --- add opencv library ------------------------------------------

add_subdirectory(${UMFELD_OPENCV_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-opencv-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-opencv)

# embed Info.plist if on macOS to allow camera access
#if (APPLE)
#    # Specify the path to your Info.plist file
#    set(INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
#
#    # Ensure the Info.plist file exists
#    if (EXISTS "${INFO_PLIST}")
#        target_link_options(${PROJECT_NAME} PRIVATE "SHELL:-sectcreate __TEXT __info_plist ${INFO_PLIST}")
#    else ()
#        message(FATAL_ERROR "Info.plist not found at ${INFO_PLIST}")
#    endif ()
#endif ()
