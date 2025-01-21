#ifndef APP_H
#define APP_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#include <string>
#include <stdio.h>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

// GLM
#include "glm/vec3.hpp" // glm::vec3

// EnTT
#include <entt/entt.hpp>
#include <entt/entity/registry.hpp>

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#endif

#ifdef __EMSCRIPTEN__
#include "emscripten_mainloop_stub.h"
#endif

// OpenGL
#include "opengl_buffer.h"
#include "opengl_shader.h"


struct AppSettings {
    std::string name = "Regman";
    // std::string font = "/remote/us01home59/bello/.fonts/FiraCode-Regular.ttf";
    std::string font = "assets/fonts/SFPro.ttf";
    float font_size  = 17.0f;

    // Window settings
    bool show_demo_window = true;       // Imgui Demo
    bool show_plot_demo_window = true;  // Implot Demo
    bool show_terminal_window = true;
};



class App {
 
    bool running;
    AppSettings settings;

    SDL_Window *window;
    SDL_WindowFlags window_flags;
    SDL_GLContext window_context;
    SDL_Event window_event;

    const char *glsl_version;

    ImVec4 window_bg_color;
    glm::vec3 triangle_color = glm::vec3(1.0f, 0.0f, 0.0f);

    OpenGLShader *mShader = nullptr;

    entt::registry mRegistry;

public:
    App();
    ~App(); 
    // Initialization
    int initializeWindow();
    int initializeUI();
    void handleEvents();

    // Rendering
    void beginRender();
    void endRender();
    void renderLoop();
    // UI
    void drawUI();
    void createMenubar();
    void createHierarchy();
    void createInspector();
    void createViewport();
    void createProfiler();
    // Render Utility Functions
    GLuint createShader(const char *shaderSource, int shaderType);

    // Clean-up
    int destroyUI();
};


#endif
