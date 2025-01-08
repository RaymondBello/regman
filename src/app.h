#ifndef APP_H
#define APP_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <string>
#include <stdio.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"


struct AppSettings {
    std::string name = "Regman";
    std::string font = "/remote/us01home59/bello/.fonts/FiraCode-Regular.ttf";
    float font_size  = 15.0f;

    // Window settings
    bool show_demo_window = true;
    bool show_terminal_window = true;
};

class App {
 
    bool running;
    AppSettings settings;

    SDL_Window *window;
    SDL_WindowFlags window_flags;
    SDL_GLContext window_context;
    SDL_Event window_event;

    ImVec4 window_bg_color;

    public:
        App();
        ~App();// Initialization
        int initializeWindow();
        int initializeUI();
        void handleEvents();
        
        // Rendering
        void beginRender();
        void drawUI();
        void endRender();
        void renderLoop();
        
        // Clean-up
        int destroyUI();
};


#endif
