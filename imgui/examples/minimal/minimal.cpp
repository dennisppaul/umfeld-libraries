#include "Umgebung.h"
#include "UmgebungImGui.h"

using namespace umgebung;

UmgebungImGui imgui_library;

void settings() {
    size(1024, 768, OPENGL);
    register_library(&imgui_library);
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
