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

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"

// #include "glm.hpp"
#include "glm/vec3.hpp" // glm::vec3

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
// #include <SDL_opengl_glext.h>
#endif


#ifdef __EMSCRIPTEN__
#include "emscripten_mainloop_stub.h"
#endif



struct AppSettings {
    std::string name = "Regman";
    std::string font = "/remote/us01home59/bello/.fonts/FiraCode-Regular.ttf";
    float font_size  = 15.0f;

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
    GLuint shaderProgram;

    const char *vertexShaderSource =
        "#version 100\n"
        "attribute vec2 aPosition;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
        "}\n";

    const char *fragmentShaderSource =
        "#version 100\n"
        "precision mediump float;"
        "uniform vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(ourColor, 1.0);\n"
        "}\n";

public:
    App();
    ~App(); // Initialization
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
