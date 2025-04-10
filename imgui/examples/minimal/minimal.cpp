#include "Umfeld.h"
#include "UmfeldImGui.h"

using namespace umfeld;

UmfeldImGui imgui_library;

void settings() {
    size(1024, 768);
    register_library(&imgui_library);
}

void draw() {
    background(0.85f);

    imgui_library.begin_frame();

    ImGui::Begin("Minimal Example");
    ImGui::Text("FPS %.1f", frameRate);
    ImGui::End();

    imgui_library.end_frame();
}
