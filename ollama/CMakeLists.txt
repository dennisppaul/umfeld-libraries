cmake_minimum_required(VERSION 3.12)

project(umfeld-library-ollama)

set(OLLAMA_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(umfeld-library-ollama INTERFACE)

target_sources(umfeld-library-ollama INTERFACE
    ${OLLAMA_PATH}/src/UmfeldOllama.cpp
)

# add curl

find_package(CURL REQUIRED)
target_link_libraries(umfeld-library-ollama INTERFACE
    CURL::libcurl
    umfeld-lib-interface
)

# link libraries

target_link_libraries(umfeld-library-ollama INTERFACE umfeld-lib-interface)
target_include_directories(umfeld-library-ollama INTERFACE
        "${OLLAMA_PATH}/src"
)
