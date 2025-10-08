/*
 * Umfeld
 *
 * This file is part of the *Umfeld* library (https://github.com/dennisppaul/umfeld).
 * Copyright (c) 2025 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Umfeld.h"
#include "Subsystems.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace umfeld {
    /**
     * see https://github.com/ocornut/imgui/wiki for Dear ImGui documentation
     */
    class UmfeldImGui final : public LibraryListener {
    public:
        enum Style {
            LIGHT,
            DARK,
            LEO
        };

        Style style{LIGHT};

        void setup_pre() override {}

        void setup_post() override {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            (void) io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

            native_renderer_type = get_native_renderer_type();
            // Setup Platform/Renderer backends
            switch (native_renderer_type) {
                case RENDERER_OPENGL_2_0: {
                    ImGui_ImplSDL3_InitForOpenGL(get_window(), get_native_renderer());
                    ImGui_ImplOpenGL2_Init();
                } break;
                case RENDERER_OPENGL_ES_3_0:
                {
                    ImGui_ImplSDL3_InitForOpenGL(get_window(), get_native_renderer());
                    const auto glsl_version = "#version 300 es";
                    ImGui_ImplOpenGL3_Init(glsl_version);
                } break;
                case RENDERER_OPENGL_3_3_CORE: {
                    ImGui_ImplSDL3_InitForOpenGL(get_window(), get_native_renderer());
                    const auto glsl_version = "#version 330";
                    ImGui_ImplOpenGL3_Init(glsl_version);
                } break;
                case RENDERER_SDL_2D: {
                    // TODO totally untested
                    ImGui_ImplSDL3_InitForSDLRenderer(get_window(), static_cast<SDL_Renderer*>(get_native_renderer()));
                    ImGui_ImplSDLRenderer3_Init(static_cast<SDL_Renderer*>(get_native_renderer()));
                } break;
                default:
                    error("Unsupported renderer type");
                    break;
            }
            // // GL ES 2.0 + GLSL 100 (WebGL 1.0)
            // const char* glsl_version = "#version 100";
            // // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
            // const char* glsl_version = "#version 300 es";

            // Setup Dear ImGui style
            switch (style) {
                case LIGHT: {
                    ImGui::StyleColorsLight();
                } break;
                case DARK: {
                    ImGui::StyleColorsDark();
                } break;
                case LEO: {
                    setup_leos_style();
                } break;
                default:
                    ImGui::StyleColorsClassic();
                    error("Unsupported style");
                    break;
            }

            context_created = true;
        }

        void update_loop() override {}

        void draw_pre() override {}

        void draw_post() override {
            // // Update and Render additional Platform Windows
            // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            //     SDL_Window*   backup_current_window  = SDL_GL_GetCurrentWindow();
            //     SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            //     ImGui::UpdatePlatformWindows();
            //     ImGui::RenderPlatformWindowsDefault();
            //     SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            // }
        }

        void begin_frame() const {
            if (native_renderer_type == NOT_INITIALIZED) {
                return;
            }

            switch (native_renderer_type) {
                case RENDERER_OPENGL_2_0: {
                    ImGui_ImplOpenGL2_NewFrame();
                } break;
                case RENDERER_OPENGL_ES_3_0:
                case RENDERER_OPENGL_3_3_CORE: {
                    ImGui_ImplOpenGL3_NewFrame();
                } break;
                case RENDERER_SDL_2D: {
                    // NOTE nothing to do
                } break;
                default:
                    error("Unsupported renderer type");
                    break;
            }

            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }

        void end_frame() const {
            if (native_renderer_type == NOT_INITIALIZED) {
                return;
            }
            ImGui::Render();
            switch (native_renderer_type) {
                case RENDERER_OPENGL_2_0: {
                    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
                } break;
                case RENDERER_OPENGL_ES_3_0:
                case RENDERER_OPENGL_3_3_CORE: {
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                } break;
                case RENDERER_SDL_2D: {
                    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), static_cast<SDL_Renderer*>(get_native_renderer()));
                } break;
                default:
                    error("Unsupported renderer type");
                    break;
            }
        }

        void event(SDL_Event* event) override {
            if (!handle_events_in_loop) {
                ImGui_ImplSDL3_ProcessEvent(event);
            }
        }

        void event_in_update_loop(SDL_Event* event) override {
            if (handle_events_in_loop) {
                ImGui_ImplSDL3_ProcessEvent(event);
            }
        }

        void set_handle_events_in_loop(const bool handle_events_in_loop) {
            this->handle_events_in_loop = handle_events_in_loop;
        }

        void load_font(const std::string& font_file_path, const float size = 13.0f) const {
            if (!context_created) {
                error("ImGUi context not created. only load fonts in setup() or after setup() has finished.");
                return;
            }
            if (!file_exists(font_file_path)) {
                error("Font file does not exist: %s", font_file_path.c_str());
                return;
            }
            const ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF(font_file_path.c_str(), size, nullptr, io.Fonts->GetGlyphRangesDefault());
        }

        void shutdown() override {
            switch (native_renderer_type) {
                case RENDERER_OPENGL_2_0: {
                    ImGui_ImplOpenGL2_Shutdown();
                } break;
                case RENDERER_OPENGL_ES_3_0:
                case RENDERER_OPENGL_3_3_CORE: {
                    ImGui_ImplOpenGL3_Shutdown();
                } break;
                case RENDERER_SDL_2D: {
                    error("TODO shutdown SDL_2D imgui?");
                } break;
                default:
                    error("Unsupported renderer type");
                    break;
            }
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
        }

    private:
        bool handle_events_in_loop{true};
        bool context_created{false};
        int  native_renderer_type{NOT_INITIALIZED};

        static void setup_leos_style() {
            auto& colors               = ImGui::GetStyle().Colors;
            colors[ImGuiCol_WindowBg]  = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
            colors[ImGuiCol_MenuBarBg] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

            // Border
            colors[ImGuiCol_Border]       = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
            colors[ImGuiCol_BorderShadow] = ImVec4{0.0f, 0.0f, 0.0f, 0.24f};

            // Text
            colors[ImGuiCol_Text]         = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
            colors[ImGuiCol_TextDisabled] = ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

            // Headers
            colors[ImGuiCol_Header]        = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
            colors[ImGuiCol_HeaderHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
            colors[ImGuiCol_HeaderActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

            // Buttons
            colors[ImGuiCol_Button]        = ImVec4{0.13f, 0.13f, 0.17, 1.0f};
            colors[ImGuiCol_ButtonHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
            colors[ImGuiCol_ButtonActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_CheckMark]     = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};

            // Popups
            colors[ImGuiCol_PopupBg] = ImVec4{0.1f, 0.1f, 0.13f, 0.92f};

            // Slider
            colors[ImGuiCol_SliderGrab]       = ImVec4{0.44f, 0.37f, 0.61f, 0.54f};
            colors[ImGuiCol_SliderGrabActive] = ImVec4{0.74f, 0.58f, 0.98f, 0.54f};

            // Frame BG
            colors[ImGuiCol_FrameBg]        = ImVec4{0.13f, 0.13, 0.17, 1.0f};
            colors[ImGuiCol_FrameBgHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
            colors[ImGuiCol_FrameBgActive]  = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

            // Tabs
            colors[ImGuiCol_Tab]                = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_TabHovered]         = ImVec4{0.24, 0.24f, 0.32f, 1.0f};
            colors[ImGuiCol_TabActive]          = ImVec4{0.2f, 0.22f, 0.27f, 1.0f};
            colors[ImGuiCol_TabUnfocused]       = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

            // Title
            colors[ImGuiCol_TitleBg]          = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_TitleBgActive]    = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};

            // Scrollbar
            colors[ImGuiCol_ScrollbarBg]          = ImVec4{0.1f, 0.1f, 0.13f, 1.0f};
            colors[ImGuiCol_ScrollbarGrab]        = ImVec4{0.16f, 0.16f, 0.21f, 1.0f};
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4{0.19f, 0.2f, 0.25f, 1.0f};
            colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4{0.24f, 0.24f, 0.32f, 1.0f};

            // Seperator
            colors[ImGuiCol_Separator]        = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};
            colors[ImGuiCol_SeparatorHovered] = ImVec4{0.74f, 0.58f, 0.98f, 1.0f};
            colors[ImGuiCol_SeparatorActive]  = ImVec4{0.84f, 0.58f, 1.0f, 1.0f};

            // Resize Grip
            colors[ImGuiCol_ResizeGrip]        = ImVec4{0.44f, 0.37f, 0.61f, 0.29f};
            colors[ImGuiCol_ResizeGripHovered] = ImVec4{0.74f, 0.58f, 0.98f, 0.29f};
            colors[ImGuiCol_ResizeGripActive]  = ImVec4{0.84f, 0.58f, 1.0f, 0.29f};

            // Docking
            // colors[ImGuiCol_DockingPreview] = ImVec4{0.44f, 0.37f, 0.61f, 1.0f};

            auto& style             = ImGui::GetStyle();
            style.TabRounding       = 4;
            style.ScrollbarRounding = 9;
            style.WindowRounding    = 7;
            style.GrabRounding      = 3;
            style.FrameRounding     = 3;
            style.PopupRounding     = 4;
            style.ChildRounding     = 4;
        }

        // TODO add multiple windows
        // #include <SDL3/SDL.h>
        // #include <SDL3/SDL_opengl.h>
        // #include "imgui.h"
        // #include "backends/imgui_impl_sdl3.h"
        // #include "backends/imgui_impl_opengl3.h"
        //
        // SDL_Window* win1;
        // SDL_Window* win2;
        // SDL_GLContext ctx1;
        // SDL_GLContext ctx2;
        // ImGuiContext* imgui1;
        // ImGuiContext* imgui2;
        //
        // void RenderWindow(SDL_Window* window, SDL_GLContext gl_ctx, ImGuiContext* imgui_ctx, const char* label) {
        //     SDL_GL_MakeCurrent(window, gl_ctx);
        //     ImGui::SetCurrentContext(imgui_ctx);
        //
        //     ImGui_ImplOpenGL3_NewFrame();
        //     ImGui_ImplSDL3_NewFrame();
        //     ImGui::NewFrame();
        //
        //     ImGui::Begin(label);
        //     ImGui::Text("This is %s", label);
        //     ImGui::End();
        //
        //     ImGui::Render();
        //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //     SDL_GL_SwapWindow(window);
        // }
        //
        // int main(int, char**) {
        //     SDL_Init(SDL_INIT_VIDEO);
        //     SDL_SetHint(SDL_HINT_OPENGL_PROFILE_MASK, "core");
        //     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        //     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        //
        //     win1 = SDL_CreateWindow("Window 1", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        //     ctx1 = SDL_GL_CreateContext(win1);
        //
        //     win2 = SDL_CreateWindow("Window 2", 200, 200, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        //     ctx2 = SDL_GL_CreateContext(win2);
        //
        //     SDL_GL_MakeCurrent(win1, ctx1);
        //     gladLoadGL();  // or your OpenGL loader
        //
        //     // Setup ImGui for window 1
        //     imgui1 = ImGui::CreateContext();
        //     ImGui::SetCurrentContext(imgui1);
        //     ImGui_ImplSDL3_InitForOpenGL(win1, ctx1);
        //     ImGui_ImplOpenGL3_Init("#version 330");
        //
        //     // Setup ImGui for window 2
        //     imgui2 = ImGui::CreateContext();
        //     ImGui::SetCurrentContext(imgui2);
        //     ImGui_ImplSDL3_InitForOpenGL(win2, ctx2);
        //     ImGui_ImplOpenGL3_Init("#version 330");
        //
        //     bool running = true;
        //     SDL_Event event;
        //     while (running) {
        //         while (SDL_PollEvent(&event)) {
        //             if (event.type == SDL_EVENT_QUIT) running = false;
        //             ImGui_ImplSDL3_ProcessEvent(&event);
        //         }
        //
        //         RenderWindow(win1, ctx1, imgui1, "Window 1");
        //         RenderWindow(win2, ctx2, imgui2, "Window 2");
        //     }
        //
        //     // Cleanup
        //     ImGui_ImplOpenGL3_Shutdown();
        //     ImGui_ImplSDL3_Shutdown();
        //     ImGui::DestroyContext(imgui2);
        //     ImGui::DestroyContext(imgui1);
        //
        //     SDL_GL_DeleteContext(ctx2);
        //     SDL_GL_DeleteContext(ctx1);
        //     SDL_DestroyWindow(win2);
        //     SDL_DestroyWindow(win1);
        //     SDL_Quit();
        //     return 0;
        // }
    };
} // namespace umfeld
