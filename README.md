# umgebung / libraries

libraries extend the core functionality of *Umgebung*.

## adding library to application

to add a library usually only the following lines need to be added at the end of the application’s CMake script ( `CMakeLists.txt` ):

```CMake
...
add_subdirectory("/absolute/path/to/library/folder" ${CMAKE_BINARY_DIR}/some-umgebung-library-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE some-umgebung-library)
```

BONUS TIP: to make things a bit cleaner it is advised to add a variable with library path at the *top* of the script:

```CMake
set(UMGEBUNG_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")     # set path to umgebung imgui library
...
```

and then later add the library:

```CMake
...
add_subdirectory(${UMGEBUNG_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/some-umgebung-library-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE some-umgebung-library)

```

a typical script for an application including the imgui library may look like this:

```CMake
cmake_minimum_required(VERSION 3.12)

project(my-application)                                               # set application name
set(UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../umgebung") # set path to umgebung library
set(UMGEBUNG_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")     # set path to umgebung imgui library

# --- no need to change anything below this line ------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")
file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(${UMGEBUNG_PATH} ${CMAKE_BINARY_DIR}/umgebung-lib-${PROJECT_NAME})
add_umgebung_libs()

# --- add imgui library -------------------------------------------------------

add_subdirectory(${UMGEBUNG_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/umgebung-imgui-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umgebung-library-imgui)
```

BONUS TIP: the second argument of `add_subdirectory()` can be any folder and is used by CMake when building. the third argument of `target_link_libraries()` denotes the library’s name and needs to be exactly the same as the name given in the library’s CMake script. usually iy is `umgebung-library-` and then the name of the library e.g `umgebung-library-imgui`.

## typical library directory structure

```
.
├── CMakeLists.txt
├── README.md
├── examples
├── include
└── src
```

## how to use a library

by default libraries only need to be instantiated and then registered in the *Umgebung* environment ( ideally in `settings()` ) with `register_library()`:

a typical library use case could look like this:


```c
#include "Umgebung.h"  
#include "UmgebungSomeLibrary.h"  
  
using namespace umgebung;  
  
UmgebungSomeLibrary some_library;  
  
void settings() {  
    size(1024, 768);  
    register_library(&some_library);  
}  
  
void setup() {  
    noFill();  
}  
  
void draw() {  
    background(0.85f);  
  
    imgui_library.begin_frame();  
  
    ImGui::Begin("Minimal Example");  
    ImGui::Text("FPS %.1f", frameRate);  
    ImGui::End();  
  
    imgui_library.end_frame();  
}
```

## how to write a library

*Umgebung* supplies a light weight mechanism for integrating libraries into the application logic. libraries are classes that implement the *interface* `LibraryListener` which includes the following methods:

```c
setup_pre()                  
setup_post()                 
draw_pre()                   
draw_post()                  
event(SDL_Event* event)
event_loop(SDL_Event* event)
shutdown()
```

an instance of a library can be registered and unregistered in *Umgebung* with the following methods:

```c
register_library(LibraryListener* listener);
unregister_library(const LibraryListener* listener);
```

thereafter the library receives the events descibed in the interface; before and after `setup()`, before and after `draw()`, for each event ( either asynchronously or in the main update loop ) or at shutdown.