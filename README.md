⚠️ THE PROJECT HAS MOVED TO CODEBERG.ORG [https://codeberg.org/Umfeld](https://codeberg.org/Umfeld) ⚠️

a collection of libraries that extend the core functionality of [umfeld](https://github.com/dennisppaul/umfeld) environment.

the repository needs to be cloned with submodules:

```sh
git clone --recurse-submodules https://github.com/dennisppaul/umfeld-libraries.git
```

## Adding Library to Application

make sure to read the library’s `README.md` file. some libraries require to install external libraries.

to add a library usually only the following lines need to be added at the end of the application’s cmake script ( `CMakeLists.txt` ):

```cmake
...
add_subdirectory("/absolute/path/to/library/folder" ${CMAKE_BINARY_DIR}/some-umfeld-library-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE some-umfeld-library)
```

bonus tip: to make things a bit cleaner it is advised to add a variable with library path at the *top* of the script:

```cmake
set(UMFELD_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")     # set path to umfeld imgui library
...
```

and then later add the library:

```cmake
...
add_subdirectory(${UMFELD_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/some-umfeld-library-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE some-umfeld-library)
```

a typical script for an application including the imgui library may look like this:

```cmake
cmake_minimum_required(VERSION 3.12)

project(my-application)                                           # set application name
set(UMFELD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umfeld") # set path to umfeld library
set(UMFELD_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")   # set path to umfeld imgui library

# --- no need to change anything below this line ------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMFELD_PATH} ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME})
add_umfeld_libs()

# --- add imgui library -------------------------------------------------------

add_subdirectory(${UMFELD_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-imgui-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-imgui)
```

bonus tip: the second argument of `add_subdirectory()` can be any folder and is used by cmake when building. the third argument of `target_link_libraries()` denotes the library’s name and needs to be exactly the same as the name given in the library’s cmake script. usually it is `umfeld-library-` and then the name of the library e.g `umfeld-library-imgui`.

## Typical Library Directory Structure

```
.
├── CMakeLists.txt
├── README.md
├── examples
├── include
└── src
```

## How to Use a Library

libraries may need to be instantiated and then registered in the *umfeld* environment ( ideally in `settings()` ) with `register_library()`. this allows the library to receive system events ( e.g key and mouse inputs ). a typical library use case could look like this:

```c
#include "Umfeld.h"  
#include "UmfeldSomeLibrary.h"  
  
using namespace umfeld;  
  
UmfeldSomeLibrary some_library;  
  
void settings() {  
    size(1024, 768);  
    register_library( &some_library );  
}  
  
void setup() {  
    noFill();  
}  
  
void draw() {  
    background(0.85f);  
  
    imgui_library.begin_frame();  
  
    ImGui::Begin( "Minimal Example" );  
    ImGui::Text( "FPS %.1f", frameRate );  
    ImGui::End();  
  
    imgui_library.end_frame();  
}
```

## How to Write a Library

*umfeld* supplies a light weight mechanism for integrating libraries into the application logic. libraries are classes that implement the *interface* `LibraryListener` which includes the following methods:

```c
setup_pre()                  
setup_post()
update_loop()
draw_pre()                   
draw_post()                  
event( SDL_Event* event )
event_loop( SDL_Event* event )
shutdown()
```

an instance of a library can be registered and unregistered in *umfeld* with the following methods:

```c
register_library( LibraryListener* listener );
unregister_library( const LibraryListener* listener );
```

thereafter the library receives the events described in the interface; before and after `setup()`, before and after `draw()`, for each event ( either asynchronously or in the main update loop ) or at shutdown.
