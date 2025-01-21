#include "app.h"

App::App() {
    printf("Info: Starting %s\n", settings.name.c_str());
    window_bg_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    running = true;
}

App::~App() {
    printf("Info: Closing App\n");
}

int App::initializeWindow() {

    int status;
    status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    if (status != 0) {
        printf("Error: Failed to initialize window: %s\n", SDL_GetError());
    }

    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif
    
    // Continue Setting up window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #else
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif


    // Create window
    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(settings.name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1400, 800, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    // Setup window context
    window_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, window_context);
    SDL_GL_SetSwapInterval(0); // Enable vsync
    return 0;
}


int App::initializeUI() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    // Setup fonts
    io.Fonts->AddFontFromFileTTF(settings.font.c_str(), settings.font_size);
    // Setup Theme
    ImGui::StyleColorsDark();
    // Setup Style
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.0f;
    }
    // Setup backend
    ImGui_ImplSDL2_InitForOpenGL(window, window_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    printf("Info: GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Create Shaders
    mShader = new OpenGLShader(vShdrSrc(default), fShdrSrc(default));
    mShader->Bind();

    float vertPositions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.f, 0.5f};
    GLuint vertPosBuffer;
    glGenBuffers(1, &vertPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    int amount = sizeof(vertPositions) / sizeof(vertPositions[0]);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(GLfloat),
                 vertPositions, GL_STATIC_DRAW);
    GLint aPositionLocation = glGetAttribLocation(mShader->m_RendererID, "aPosition");
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(aPositionLocation);

    

    return 0;
}

void App::handleEvents() {
       while (SDL_PollEvent(&window_event)) {
            ImGui_ImplSDL2_ProcessEvent(&window_event);
            if (window_event.type == SDL_QUIT) {
                running = false;
            }
            if (window_event.type == SDL_WINDOWEVENT && window_event.window.event == SDL_WINDOWEVENT_CLOSE && window_event.window.windowID == SDL_GetWindowID(window)) {
                running = false;
            }
       }
       if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
       }

}

void App::beginRender() {
    // Start Render Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

}

void App::endRender() {
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

    glClearColor(window_bg_color.x * window_bg_color.w, window_bg_color.y * window_bg_color.w, window_bg_color.z * window_bg_color.w, window_bg_color.w);
    
    glClear(GL_COLOR_BUFFER_BIT);

    int colorLocation = glGetUniformLocation(mShader->m_RendererID, "ourColor");
    glUniform3f(colorLocation, triangle_color.x, triangle_color.y, triangle_color.z);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
    SDL_GL_SwapWindow(window);
}

int App::destroyUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(window_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void App::createMenubar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Create New Scene", "Ctrl+N"))
            {
                printf("Create New Scene\n");
            }
            if (ImGui::MenuItem("Load Scene", "Ctrl+O"))
            {
                printf("Load Scene\n");
            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
            {
                printf("Save Scene\n");
            }
            if (ImGui::MenuItem("Save Scene as..", "Ctrl+Shift+S"))
            {
                printf("Save Scene as..\n");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Undo", "CTRL+Z");
            ImGui::MenuItem("Redo", "CTRL+Y");
            ImGui::Separator();
            ImGui::MenuItem("Cut", "CTRL+X");
            ImGui::MenuItem("Copy", "CTRL+C");
            ImGui::MenuItem("Paste", "CTRL+V");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Terminal", NULL, &settings.show_terminal_window);
            ImGui::MenuItem("Demo Window", NULL, &settings.show_demo_window);
            ImGui::MenuItem("Plot Demo Window", NULL, &settings.show_plot_demo_window);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void App::createHierarchy() {
}

void App::drawUI() {

    createMenubar();

    if (settings.show_demo_window) {
        ImGui::ShowDemoWindow(&settings.show_demo_window);
    }
    if (settings.show_plot_demo_window) {
        ImPlot::ShowDemoWindow(&settings.show_plot_demo_window);
    }
    if (settings.show_terminal_window) {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Terminal");
        ImGui::Checkbox("Demo Window", &settings.show_demo_window);
        ImGui::ColorEdit3("BG Color", (float*)&window_bg_color);
        ImGui::ColorEdit3("Tri Color", (float *)&triangle_color);
        ImGui::Text("Application average %.3f ms/frame %.1f fps", 1000.0f / io.Framerate, io.Framerate);

        static char buf1[256] = "ls -al"; 
        ImGui::InputText("Command-line",buf1, 256);
        static std::string text;

        if (ImGui::Button("Send")) {
            printf("Command: %s\n", buf1);
        }

        ImGui::End();
    }
}
    
void App::renderLoop() {
    #ifdef __EMSCRIPTEN__
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.IniFilename = nullptr;
    MainLoopForEmscriptenP = [&]() { do
    #else 
    while (running)
    #endif
    {
        handleEvents();
        beginRender();
        drawUI();
        endRender();
    }
    #ifdef __EMSCRIPTEN__
    while (0); };
    emscripten_set_main_loop(MainLoopForEmscripten, 0, true);
    #endif
}

// Render Functions
GLuint App::createShader(const char *shaderSource, int shaderType){
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        glDeleteShader(shader); // Don't leak the shader
        printf("%s\n", &errorLog[0]);
        printf("%s\n", shaderSource);
    }
    return shader;
}