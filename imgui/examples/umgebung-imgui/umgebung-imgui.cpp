#include "Umgebung.h"
#include "Subsystems.h"
#include "UmgebungImGui.h"

using namespace umgebung;

UmgebungImGui imgui_library;
bool          show_demo_window    = false;
bool          show_another_window = false;
bool          my_tool_active      = true;
auto          clear_color         = glm::vec3(0.45f, 0.55f, 0.60f);
auto          my_color            = glm::vec4(0.45f, 0.55f, 0.60f, 1.0f);

void settings() {
    size(1024, 768, OPENGL);
    register_library(&imgui_library);
    imgui_library.style = UmgebungImGui::Style::LIGHT;
}

void setup() {
    noFill();
    stroke(1.0f, 0.25f, 0.35f);
}

void draw() {
    background(0.85f);
    const float size = 50.0f;
    const float x    = width / 2.0f;
    const float y    = height / 2.0f;
    line(x - size, y - size, x + size, y + size);
    line(x - size, y + size, x + size, y - size);


    imgui_library.begin_frame();

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    { // from https://github.com/ocornut/imgui/tree/master/examples/example_sdl3_opengl3
        static float f       = 0.0f;
        static int   counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*) &clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) { // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / frameRate, frameRate);
        ImGui::End();
    }

    { // from https://github.com/ocornut/imgui/tree/master

        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
                }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Edit a color stored as 4 floats
        ImGui::ColorEdit4("Color", (float*) &my_color);

        // Generate samples and plot them
        float samples[100];
        for (int n = 0; n < 100; n++) {
            samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
        }
        ImGui::PlotLines("Samples", samples, 100);

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = 0; n < 50; n++) {
            ImGui::Text("%04d: Some text", n);
        }
        ImGui::EndChild();
        ImGui::End();
    }

    imgui_library.end_frame();
}
