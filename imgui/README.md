# Umfeld / Library / Dear ImGui

to use *Dear ImGui* in *Umfeld* include the library header and create an instance:

```c
#include "UmfeldImGui.h"

UmfeldImGui imgui_library;
```

register the library in `settings()`:

```c
void settings() {
    // ...
    register_library(&imgui_library);
}
```

configure the library in `setup()` ( e.g loading fonts or setting styles ):

```c
void setup() {
    imgui_library.load_font(sketchPath() + "../RobotoMono-Regular.ttf", 32);
    imgui_library.style = UmfeldImGui::Style::LEO;
}
```

and finally use *Dear ImGui* functions somewhere in `draw()` but always in between `imgui_library.begin_frame()` and `imgui_library.end_frame()` e.g:

```c
void draw() {
    // ...
    imgui_library.begin_frame();

    ImGui::Begin("Loading Custom Font Example");
    ImGui::Text("FPS %.1f", frameRate);
    ImGui::End();

    imgui_library.end_frame();
    // ...
}
```

for more information on how to actually use *Dear ImGui* itself, see the documentation e.g at https://github.com/ocornut/imgui or https://github.com/ocornut/imgui/wiki for Dear ImGui documentation or run the demo window example ( i.e `imgui-demo` ).

## Include Library

to include the library, add the following lines to your application’s CMake script:

```cmake
set(UMFELD_IMGUI_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")   # set path to umfeld imgui library

add_subdirectory(${UMFELD_IMGUI_LIB_PATH} ${CMAKE_BINARY_DIR}/umfeld-imgui-${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-library-imgui)
```
