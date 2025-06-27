# Umfeld / Library / OpenCV

## Prepare

install OpenCV `@version(macOS, 4.10.0)`:

* on *macOS*: use Homebrew `brew install opencv`
* on *Linux*: use APT `sudo apt install libopencv-dev`
* on *Windows ( MSYS2 UCRT64 )*: use Pacman `pacman -S mingw-w64-ucrt-x86_64-opencv`

to test the installation, run `opencv_version` in the terminal.

## Include Library

to include the library, add the following lines to your application’s CMake script:

```cmake
set(OPENCV_UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umfeld-libraries/opencv") # path to this library

add_subdirectory(${OPENCV_UMFELD_PATH}
        ${CMAKE_BINARY_DIR}/umfeld-opencv-${PROJECT_NAME} # can be any path to a directory
)

target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-opencv) # link library to project
```
