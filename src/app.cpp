#include "app.h"

App::App() {
    printf("Info: Starting %s\n", settings.name.c_str());
    window_bg_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create window
    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(settings.name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
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
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup fonts
    io.Fonts->AddFontFromFileTTF(settings.font.c_str(), settings.font_size);
    // Setup Theme
    ImGui::StyleColorsDark();
    // Setup backend
    ImGui_ImplSDL2_InitForOpenGL(window, window_context);
    ImGui_ImplOpenGL2_Init();

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
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

}

void App::endRender() {
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(window_bg_color.x * window_bg_color.w, window_bg_color.y * window_bg_color.w, window_bg_color.z * window_bg_color.w, window_bg_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    // glUseProgram(0);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

int App::destroyUI() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(window_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

std::string execute(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

void App::drawUI() {
    if (settings.show_demo_window) {
        ImGui::ShowDemoWindow(&settings.show_demo_window);
    }
    if (settings.show_terminal_window) {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::Begin("Terminal");
        ImGui::Checkbox("Demo Window", &settings.show_demo_window);
        ImGui::ColorEdit3("BG Color", (float*)&window_bg_color);
        ImGui::Text("Application average %.3f ms/frame %.1f fps", 1000.0f / io.Framerate, io.Framerate);

        static char buf1[256] = "ls -al"; ImGui::InputText("Command-line",buf1, 256);
        static std::string text;

        if (ImGui::Button("Send")) {
            text = execute(buf1);
        }
        ImGui::Text(text.c_str());

        ImGui::End();
    }
}
    
void App::renderLoop() {
    while (running) {
        handleEvents();
        beginRender();
        drawUI();
        endRender();
    }
}
