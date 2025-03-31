#include "Umgebung.h"
#include "UmgebungImGui.h"

using namespace umgebung;

UmgebungImGui imgui_library;

void settings() {
    size(1024, 768);
    register_library(&imgui_library);
}

void draw() {
    background(0.85f);

    imgui_library.begin_frame();
    ImGui::ShowDemoWindow();
    imgui_library.end_frame();
}
