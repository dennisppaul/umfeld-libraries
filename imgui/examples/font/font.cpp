#include "Umfeld.h"
#include "UmfeldImGui.h"

using namespace umfeld;

UmfeldImGui imgui_library;

void settings() {
    size(1024, 768);
    register_library(&imgui_library);
}

void setup() {
    imgui_library.load_font(sketchPath() + "../RobotoMono-Regular.ttf", 32);
}

void draw() {
    background(0.85f);

    imgui_library.begin_frame();

    ImGui::Begin("Loading Custom Font Example");
    ImGui::Text("FPS %.1f", frameRate);
    ImGui::End();

    imgui_library.end_frame();
}
