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
    glEnable(GL_DEPTH_TEST);

    // Create a scene
    m_SelectedEntityId = -1;
    m_Scene = std::make_shared<Scene>();
    m_Scene->BeginScene();
    m_Scene->BeginEditor();

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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ////////////// RENDER START //////////////

    // int colorLocation = glGetUniformLocation(mShader->m_RendererID, "ourColor");
    // glUniform3f(colorLocation, triangle_color.x, triangle_color.y, triangle_color.z);

    auto cameraView = m_Scene->Reg().view<CameraComponent>();
    for (auto entity : cameraView)
    {
        auto &camera = cameraView.get<CameraComponent>(entity);
        if (camera.isPrimary)
        {

            for (auto &&[entity, mesh, transform] : m_Scene->Reg().view<MeshComponent, TransformComponent>().each())
            {
                if (mesh.shader.instance != nullptr)
                {
                    mesh.shader.instance->Bind();

                    // Update Model Matrix
                    transform.rotation.x += io.DeltaTime * 40.0f;
                    transform.rotation.y += io.DeltaTime * 20.0f;

                    // reset rotation to 0
                    if (transform.rotation.x > 360.0f)
                        transform.rotation.x = 0.0f;
                    if (transform.rotation.y > 360.0f)
                        transform.rotation.y = 0.0f;

                    // Update Model Matrix
                    mesh.shader.instance->UploadUniformMat4("uModelMatrix", transform.getMatrix());

                    // Update View Matrix
                    auto viewMatrix = glm::mat4(1.0f);
                    // viewMatrix = glm::lookAt(m_position, m_position + m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

                    // camera.camera.updateCameraSize(displaySize.x, displaySize.y);
                    // auto viewMatrix = camera.camera.getViewMatrix();

                    mesh.shader.instance->UploadUniformMat4("uViewMatrix", viewMatrix);

                    // Update Projection Matrix
                    ImVec2 displaySize = ImVec2(io.DisplaySize.x * io.DisplayFramebufferScale.x,
                                                io.DisplaySize.y * io.DisplayFramebufferScale.y);
                    auto projectionMatrix = glm::perspective(glm::radians(90.0f), displaySize.x / displaySize.y, 0.1f, 100.0f);
                    // auto projectionMatrix = glm::ortho(-3.0f, 3.0f, -2.0f, 2.0f, -1.0f, 1.0f);
                    // projectionMatrix = glm::mat4(1.0f);
                    mesh.shader.instance->UploadUniformMat4("uProjectionMatrix", projectionMatrix);

                    // Check if mesh has a texture
                    if (mesh.hasTexture) {
                        int slot = mesh.texture.instance->m_TexSlot;
                        mesh.shader.instance->UploadUniformInt("tex0", slot);
                        mesh.texture.instance->Bind();
                    }

                    // Bind the VAO for this mesh
                    mesh.vao->Bind();

                    // Setup Face culling and depth testing
                    // glEnable(GL_DEPTH_TEST);

                    // glEnable(GL_CULL_FACE);
                    // glCullFace(GL_BACK);

                    glDrawElements(GL_TRIANGLES, mesh.vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

                    // glDisable(GL_CULL_FACE);
                    // glDisable(GL_DEPTH_TEST);

                    mesh.vao->Unbind();
                    mesh.shader.instance->Unbind();
                    if (mesh.hasTexture)
                        mesh.texture.instance->UnBind();
                }
            }
        }
    }

    // glDrawArrays(GL_TRIANGLES, 0, 3);

    ////////////// RENDER END //////////////

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
            ImGui::MenuItem("Scene Graph", NULL, &settings.show_scene_graph_window);
            ImGui::MenuItem("Inspector", NULL, &settings.show_inspector_window);
            ImGui::MenuItem("Text Editor", NULL, &settings.show_editor_window);

            ImGui::EndMenu();
        }
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::Text("avg %.3f ms/frame %.1f fps", 1000.0f / io.Framerate, io.Framerate);

        ImGui::EndMainMenuBar();
    }
}

void App::createSceneGraph()
{

    static int selectionMask = (1 << 0);
    int node_clicked = -1;

    static uint32_t selectedEntity = -1;
    uint32_t entityClicked = -1;

    ImGui::Begin("Scene Graph");

    if (ImGui::Button("Create Object", ImVec2(-1, 0)))
    {
        // Create popup menu with list of available components
        ImGui::OpenPopup("create_object");
    }

    if (ImGui::BeginPopup("create_object"))
    {
        for (const auto &pair : ObjectTypeStringMap)
        {
            if (ImGui::Selectable(pair.second.c_str()))
            {
                m_Scene->AddObject(pair.first);
            }
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();

    m_Scene->Reg().view<TagComponent>().each([&entityClicked](auto entity, auto &tag)
                                             {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool isSelected = (selectedEntity == (uint32_t)entity);
        if (isSelected)
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)entity, node_flags, "%s", tag.tag.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            entityClicked = (uint32_t)entity;
        }
        if (node_open)
        {
            ImGui::Text("Tag: %s", tag.tag.c_str());
            ImGui::TreePop();
        } });

    if (entityClicked != -1)
    {
        // Update selection state
        // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
        if (ImGui::GetIO().KeyCtrl)
        {
            selectedEntity = -1; // CTRL+click to toggle
            m_SelectedEntityId = -1;
        }
        else // if (!(selectionMask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
        {
            selectedEntity = entityClicked; // Click to single-select
            m_SelectedEntityId = selectedEntity;
        }
    }
    if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && !ImGui::IsAnyItemHovered())
    {
        selectedEntity = -1;
        m_SelectedEntityId = -1;
    }
    ImGui::End();
}

void App::createInspector()
{

    ImGui::Begin("Inspector");

    // Get entity
    if (m_SelectedEntityId != -1)
    {
        // Get entity
        auto currentEntity = Entity((entt::entity)m_SelectedEntityId, m_Scene.get());

        // Create popup menu with list of available components
        bool addComponent = false;

        if (ImGui::Button("Add component", ImVec2(-1, 0)))
        {
            // Create popup menu with list of available components
            ImGui::OpenPopup("Add Component");
        }

        if (ImGui::BeginPopup("Add Component"))
        {
            if (ImGui::Selectable("Transform Component"))
            {
                // Check if component already exists
                bool hasComponent = currentEntity.HasComponent<TransformComponent>();

                if (!hasComponent)
                    currentEntity.AddComponent<TransformComponent>();
                else
                {
                    auto tag = currentEntity.GetComponent<TagComponent>().tag;
                    printf("Error: #%0u (%s): Transform component already exists!", (uint32_t)currentEntity.ID(), tag.c_str());
                }
            }
            if (ImGui::Selectable("Camera Component"))
            {
                // Check if component already exists
                bool hasComponent = currentEntity.HasComponent<CameraComponent>();

                if (!hasComponent)
                    currentEntity.AddComponent<CameraComponent>();
                else
                {
                    auto tag = currentEntity.GetComponent<TagComponent>().tag;
                    printf("Error: #%0u (%s): Camera component already exists!", (uint32_t)currentEntity.ID(), tag.c_str());
                }
            }
            ImGui::EndPopup();
        }

        ImGui::Separator();

        if (m_Scene->Reg().any_of<TagComponent>((entt::entity)m_SelectedEntityId))
        {
            // Get tag
            static bool allowEditing = true;

            static std::string tag;
            auto &tagComponent = m_Scene->Reg().get<TagComponent>((entt::entity)m_SelectedEntityId);
            // tag = tagComponent.tag;

            char buffer[256];
            std::strncpy(buffer, tagComponent.tag.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0'; // Null-terminate

            ImGui::Checkbox("##", &allowEditing);
            ImGui::SameLine();

            if (ImGui::InputText("Name", buffer, sizeof(buffer), allowEditing ? 0 : ImGuiInputTextFlags_ReadOnly))
                tagComponent.tag = buffer;

            ImGui::Separator();
        }

        if (m_Scene->Reg().any_of<TransformComponent>((entt::entity)m_SelectedEntityId))
        {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("Position", glm::value_ptr(m_Scene->Reg().get<TransformComponent>((entt::entity)m_SelectedEntityId).position), 0.1f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(m_Scene->Reg().get<TransformComponent>((entt::entity)m_SelectedEntityId).rotation), 1.0f);
                ImGui::DragFloat3("Scale", glm::value_ptr(m_Scene->Reg().get<TransformComponent>((entt::entity)m_SelectedEntityId).scale), 0.1f);
                ImGui::Separator();
            }
        }

        if (m_Scene->Reg().any_of<CameraComponent>((entt::entity)m_SelectedEntityId))
        {
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto cameraEntity = Entity((entt::entity)m_SelectedEntityId, m_Scene.get());

                ImGui::DragFloat3("Camera Position", glm::value_ptr(cameraEntity.GetComponent<CameraComponent>().camera.position), 0.1f);
                ImGui::DragFloat3("Front Vector", glm::value_ptr(cameraEntity.GetComponent<CameraComponent>().camera.frontVector), 0.1f, -1.0f, 1.0f);
                ImGui::DragFloat3("Up Vector", glm::value_ptr(cameraEntity.GetComponent<CameraComponent>().camera.upVector), 0.1f, -1.0f, 1.0f);
                ImGui::DragFloat("Field of View", &cameraEntity.GetComponent<CameraComponent>().camera.fieldOfView, 1.0f, 45, 150.0f);
                ImGui::DragFloat("Look Sensitivity", &cameraEntity.GetComponent<CameraComponent>().camera.lookSensitivity, 0.01f, 0.01f, 50.0f);
                ImGui::DragFloat("Zoom Sensitivity", &cameraEntity.GetComponent<CameraComponent>().camera.zoomSensitivity, 0.01f, 0.01f, 20.0f);
                ImGui::DragFloat("Move Sensitivity", &cameraEntity.GetComponent<CameraComponent>().camera.moveSensitivity, 1.0f, 1.0f, 100.0f);

                ImGui::Checkbox("Primary Camera", &cameraEntity.GetComponent<CameraComponent>().isPrimary);
                ImGui::Separator();
            }
        }
    }
    else
    {
        std::string noEntitySelected = "No entity selected";
        std::string deselect = "CTRL+RClick to deselect";
        // Center the text in the window
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(noEntitySelected.c_str()).x) / 2);
        ImGui::Text("%s", noEntitySelected.c_str());
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(deselect.c_str()).x) / 2);
        ImGui::Text("%s", deselect.c_str());

        ImGui::Separator();
        ImGui::Text("Camera");
        // ImGui::DragFloat3("Look At", glm::value_ptr(m_lookAt), 0.01f);
        // ImGui::DragFloat("Mouse Speed", &m_mouseSpeed, 0.01f, 0.1f, 1.0f);
        // ImGui::DragFloat("Movement Speed", &m_moveSpeed, 0.01f, 0.1f, 50.0f);
        // ImGui::DragFloat3("Position", glm::value_ptr(m_position), 0.1f);
        // ImGui::DragFloat3("Rotation", glm::value_ptr(m_rotation), 1.0f);
        // ImGui::DragFloat3("Scale", glm::value_ptr(m_scale), 0.1f);
    }

    ImGui::End();
}

void App::createEditor()
{

    if (m_Scene)
    {
        auto cpos = m_Scene->editor.GetCursorPosition();
        ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
        ImGui::SetWindowSize(ImVec2(800, 650), ImGuiCond_FirstUseEver);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    auto textToSave = m_Scene->editor.GetText();
                    printf("Info: File content: %s", textToSave.c_str());
                    /// save text....
                }
                if (ImGui::MenuItem("Quit", "Alt-F4"))
                    settings.show_editor_window = false;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                bool ro = m_Scene->editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    m_Scene->editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && m_Scene->editor.CanUndo()))
                    m_Scene->editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && m_Scene->editor.CanRedo()))
                    m_Scene->editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, m_Scene->editor.HasSelection()))
                    m_Scene->editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && m_Scene->editor.HasSelection()))
                    m_Scene->editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_Scene->editor.HasSelection()))
                    m_Scene->editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                    m_Scene->editor.Paste();

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    m_Scene->editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_Scene->editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Dark palette"))
                    m_Scene->editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    m_Scene->editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    m_Scene->editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_Scene->editor.GetTotalLines(),
                    m_Scene->editor.IsOverwrite() ? "Ovr" : "Ins",
                    m_Scene->editor.CanUndo() ? "*" : " ",
                    m_Scene->editor.GetLanguageDefinition().mName.c_str(), m_Scene->fileToEdit);
        m_Scene->editor.Render("Text Editor");

        ImGui::End();
    }
}

void App::drawUI() {

    createMenubar();

    if (settings.show_demo_window)
        ImGui::ShowDemoWindow(&settings.show_demo_window);

    if (settings.show_plot_demo_window)
        ImPlot::ShowDemoWindow(&settings.show_plot_demo_window);

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

    if (settings.show_scene_graph_window)
        createSceneGraph();

    if (settings.show_inspector_window)
        createInspector();

    if (settings.show_editor_window)
        createEditor();
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