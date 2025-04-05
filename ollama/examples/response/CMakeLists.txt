cmake_minimum_required(VERSION 3.12)

project(response)                                                     # set application name
set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umgebung") # set path to umgebung library
set(UMGEBUNG_OLLAMA_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")    # set path to umgebung ollama library

# --- no need to change anything below this line ------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# --- add ollama library ------------------------------------------------------

add_subdirectory(${UMGEBUNG_OLLAMA_LIB_PATH} ${CMAKE_BINARY_DIR}/umgebung-ollama-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umgebung-library-ollama)
