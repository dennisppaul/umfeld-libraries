cmake_minimum_required(VERSION 3.12)

project(minimal)                                                  # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umfeld") # set path to umfeld library
set(UMFELD_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")   # set path to umfeld imgui library

# --- no need to change anything below this line ------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# enable OpenGL ES 3.0 support on macOS via ANGLE
#option(ENABLE_ANGLE "Enable ANGLE OpenGL ES emulation on macOS ( apple silicon )" OFF) # if on macOS, use ANGLE to get OpenGL ES support via Metal
#target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_ES3)
#set(UMFELD_OPENGL_VERSION "OPENGL_ES_3_0")

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# --- add imgui library -------------------------------------------

add_subdirectory(${UMFELD_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-imgui-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-imgui)
