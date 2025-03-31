# umgebung / library / Dear ImGui

to use *Dear ImGui* in *Umgebung* include the library header and create an instance:

```c
#include "UmgebungImGui.h"

UmgebungImGui imgui_library;
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
    imgui_library.style = UmgebungImGui::Style::LEO;
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
