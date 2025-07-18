cmake_minimum_required(VERSION 3.12)

project(response)                                                 # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umfeld") # set path to umfeld library
set(UMFELD_OLLAMA_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")  # set path to umfeld ollama library

# --- no need to change anything below this line ------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# --- add ollama library ------------------------------------------

add_subdirectory(${UMFELD_OLLAMA_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-ollama-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-ollama)
