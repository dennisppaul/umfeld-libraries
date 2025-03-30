/*
 * Umgebung
 *
 * This file is part of the *Umgebung* library (https://github.com/dennisppaul/umgebung).
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

#include "Umgebung.h"
#include "Subsystems.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace umgebung {

    /**
     * see https://github.com/ocornut/imgui/wiki for Dear ImGui documentation
     */
    class UmgebungImGui final : public LibraryListener {
    public:
        enum Style {
            LIGHT,
            DARK,
            LEO
        };

        Style style{LIGHT};

        void setup_pre() override {
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
                case OPENGL_2_0: {
                    ImGui_ImplSDL3_InitForOpenGL(get_window(), get_native_renderer());
                    ImGui_ImplOpenGL2_Init();
                } break;
                case OPENGL_3_3: {
                    ImGui_ImplSDL3_InitForOpenGL(get_window(), get_native_renderer());
                    const auto glsl_version = "#version 150";
                    ImGui_ImplOpenGL3_Init(glsl_version);
                } break;
                case SDL_2D: {
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
                    setup_default_style();
                } break;
                default:
                    ImGui::StyleColorsClassic();
                    error("Unsupported style");
                    break;
            }
        }

        void setup_post() override {}

        void draw_pre() override {}

        void draw_post() override {
            // ImGuiIO& io = ImGui::GetIO();
            // (void) io;
            // ImGui::Render();
            // // glUseProgram(0);
            // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            // // // Update and Render additional Platform Windows
            // // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            // //     SDL_Window*   backup_current_window  = SDL_GL_GetCurrentWindow();
            // //     SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            // //     ImGui::UpdatePlatformWindows();
            // //     ImGui::RenderPlatformWindowsDefault();
            // //     SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            // // }
        }

        static void begin_frame() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }

        void end_frame() const {
            ImGui::Render();
            switch (native_renderer_type) {
                case OPENGL_2_0: {
                    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
                } break;
                case OPENGL_3_3: {
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                } break;
                case SDL_2D: {
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

        void event_loop(SDL_Event* event) override {
            if (handle_events_in_loop) {
                ImGui_ImplSDL3_ProcessEvent(event);
            }
        }

        void shutdown() override {
            switch (native_renderer_type) {
                case OPENGL_2_0: {
                    ImGui_ImplOpenGL2_Shutdown();
                } break;
                case OPENGL_3_3: {
                    ImGui_ImplOpenGL3_Shutdown();
                } break;
                case SDL_2D: {
                    error("TODO shutdown SDL_2D imgui?");
                } break;
                default:
                    error("Unsupported renderer type");
                    break;
            }
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
        }

        void set_handle_events_in_loop(const bool handle_events_in_loop) {
            this->handle_events_in_loop = handle_events_in_loop;
        }

    private:
        bool handle_events_in_loop = true;
        int  native_renderer_type{DEFAULT};

        static void setup_default_style() {
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
    };
} // namespace umgebung
