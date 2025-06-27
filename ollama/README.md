# Umfeld / Library / Ollama

## Prepare

install ollama `@version(macOS, latest)`:

* on *macOS*: use homebrew `brew install ollama`
* on *Linux*: use apt `curl -fsSL https://ollama.com/install.sh | sh`
* on *Windows ( MSYS2 UCRT64 )*: Ollama is not officially supported under MSYS2. use the native Windows installer from [ollama.com](https://ollama.com) or run ollama in WSL2.

to test the installation, run `ollama run llama2` in the terminal ( this downloads and starts a test model ).

## Include Library

to include the library, add the following lines to your application’s CMake script:

```cmake
set(UMFELD_OLLAMA_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")  # set path to umfeld ollama library

add_subdirectory(${UMFELD_OLLAMA_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-ollama-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-ollama)
```
