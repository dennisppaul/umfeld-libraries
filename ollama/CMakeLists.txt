cmake_minimum_required(VERSION 3.12)

project(umgebung-library-ollama)

set(OLLAMA_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(umgebung-library-ollama INTERFACE)

target_sources(umgebung-library-ollama INTERFACE
    ${OLLAMA_PATH}/src/UmgebungOllama.cpp
)

# add curl

find_package(CURL REQUIRED)
target_link_libraries(umgebung-library-ollama INTERFACE CURL::libcurl)

# link libraries

target_link_libraries(umgebung-library-ollama INTERFACE umgebung-lib-interface)
target_include_directories(umgebung-library-ollama INTERFACE
        "${OLLAMA_PATH}/src"
)
