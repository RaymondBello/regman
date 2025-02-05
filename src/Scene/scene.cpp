#include "scene.h"
#include "entity.h"

Scene::Scene(/* args */)
{
    m_ActiveCamera = Camera(16, 9);
    printf("Info: Scene Created\n");
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity(std::string name)
{
    // Create entity
    Entity entity = {m_Registry.create(), this};

    // Add mandatory components
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<TransformComponent>();

    return entity;
}

Entity Scene::CreateRawEntity(std::string name)
{
    // Create entity
    Entity entity = {m_Registry.create(), this};

    // Add mandatory components
    entity.AddComponent<TagComponent>(name);

    return entity;
}

Entity Scene::AddObject(ObjectType type)
{
    Entity newEntity;

    switch (type)
    {
    case ObjectType::CUBE:
    {
        newEntity = CreateEntity("New Cube");
        auto shader = Shader{
            "Default Shader",
            new OpenGLShader(vShdrSrc(default), fShdrSrc(default))};
        BufferLayout layout = {
            {ShaderDataType::Float3, "aPosition"},
            {ShaderDataType::Float3, "aColor"}};
        auto meshComponent = MeshComponent(shader, cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), layout);

        newEntity.AddComponent<MeshComponent>(meshComponent);

        break;
    }
    case ObjectType::PYRAMID:
    {
        newEntity = CreateEntity("New Pyramid");
        auto shader = Shader{
            "Default Shader",
            new OpenGLShader(vShdrSrc(default), fShdrSrc(default))};
        BufferLayout layout = {
            {ShaderDataType::Float3, "aPosition"},
            {ShaderDataType::Float3, "aColor"}};
        auto meshComponent = MeshComponent(shader, pyramidVertices, sizeof(pyramidVertices), pyramidIndices, sizeof(pyramidIndices), layout);

        newEntity.AddComponent<MeshComponent>(meshComponent);

        break;
    }

    default:
        printf("ERROR: Could not create object\n");
        break;
    }

    return newEntity;
}

void Scene::BeginScene()
{
    glm::vec3 start_point = {0.0f, 0.0f, -1.7f};
    glm::vec3 spacing = {0.0f, 0.1f, 0.0f};

    // Create entities
    // auto cubeEntity = AddObject(ObjectType::CUBE);
    // cubeEntity.GetComponent<TransformComponent>().position = start_point;

    auto pyramidEntity = AddObject(ObjectType::PYRAMID);
    pyramidEntity.GetComponent<TransformComponent>().position = start_point + spacing;

    // Create the plane
    // auto planeEntity = CreateEntity("Plane");
    // auto planeshader = Shader{
    //     "Default Shader",
    //     new OpenGLShader(vShdrSrc(default), fShdrSrc(default))};
    // BufferLayout planeLayout = {
    //     {ShaderDataType::Float3, "a_Position"},
    //     {ShaderDataType::Float3, "a_Color"}};
    // auto planeMeshComponent = MeshComponent(planeshader, planeVertices, sizeof(planeVertices), planeIndices, sizeof(planeIndices), planeLayout);

    auto cameraEntity = CreateRawEntity("Scene Camera");
    auto &camera = cameraEntity.AddComponent<CameraComponent>(16.0f, 9.0f);
    camera.isPrimary = true;
}

void Scene::BeginEditor()
{
    auto lang = TextEditor::LanguageDefinition::CPlusPlus();

    // set your own known preprocessor symbols...
    static const char *ppnames[] = {"NULL", "PM_REMOVE",
                                    "ZeroMemory", "DXGI_SWAP_EFFECT_DISCARD", "D3D_FEATURE_LEVEL", "D3D_DRIVER_TYPE_HARDWARE", "WINAPI", "D3D11_SDK_VERSION", "assert"};
    // ... and their corresponding values
    static const char *ppvalues[] = {
        "#define NULL ((void*)0)",
        "#define PM_REMOVE (0x0001)",
        "Microsoft's own memory zapper function\n(which is a macro actually)\nvoid ZeroMemory(\n\t[in] PVOID  Destination,\n\t[in] SIZE_T Length\n); ",
        "enum DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD = 0",
        "enum D3D_FEATURE_LEVEL",
        "enum D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE  = ( D3D_DRIVER_TYPE_UNKNOWN + 1 )",
        "#define WINAPI __stdcall",
        "#define D3D11_SDK_VERSION (7)",
        " #define assert(expression) (void)(                                                  \n"
        "    (!!(expression)) ||                                                              \n"
        "    (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \n"
        " )"};

    for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
    {
        TextEditor::Identifier id;
        id.mDeclaration = ppvalues[i];
        lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
    }

    // set your own identifiers
    static const char *identifiers[] = {
        "HWND", "HRESULT", "LPRESULT", "D3D11_RENDER_TARGET_VIEW_DESC", "DXGI_SWAP_CHAIN_DESC", "MSG", "LRESULT", "WPARAM", "LPARAM", "UINT", "LPVOID",
        "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
        "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
        "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "TextEditor"};
    static const char *idecls[] =
        {
            "typedef HWND_* HWND", "typedef long HRESULT", "typedef long* LPRESULT", "struct D3D11_RENDER_TARGET_VIEW_DESC", "struct DXGI_SWAP_CHAIN_DESC",
            "typedef tagMSG MSG\n * Message structure", "typedef LONG_PTR LRESULT", "WPARAM", "LPARAM", "UINT", "LPVOID",
            "ID3D11Device", "ID3D11DeviceContext", "ID3D11Buffer", "ID3D11Buffer", "ID3D10Blob", "ID3D11VertexShader", "ID3D11InputLayout", "ID3D11Buffer",
            "ID3D10Blob", "ID3D11PixelShader", "ID3D11SamplerState", "ID3D11ShaderResourceView", "ID3D11RasterizerState", "ID3D11BlendState", "ID3D11DepthStencilState",
            "IDXGISwapChain", "ID3D11RenderTargetView", "ID3D11Texture2D", "class TextEditor"};

    for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
    {
        TextEditor::Identifier id;
        id.mDeclaration = std::string(idecls[i]);
        lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
    }
    editor.SetLanguageDefinition(lang);
    // editor.SetPalette(TextEditor::GetLightPalette());

    // error markers
    // TextEditor::ErrorMarkers markers;
    // markers.insert(std::make_pair<int, std::string>(6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
    // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
    // editor.SetErrorMarkers(markers);

    // "breakpoint" markers
    // TextEditor::Breakpoints bpts;
    // bpts.insert(24);
    // bpts.insert(47);
    // editor.SetBreakpoints(bpts);

    //	static const char* fileToEdit = "test.cpp";

    {
        std::ifstream t(fileToEdit);
        if (t.good())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            editor.SetText(str);
        }
        else
        {
            printf("Error: Failed to open file %s", fileToEdit);
        }
    }
}

void Scene::RenderScene(float width, float height)
{
    // Get Primary camera
    auto &io = ImGui::GetIO();

    Camera renderCam;

    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view)
    {
        auto &cameraComponent = view.get<CameraComponent>(entity);
        if (cameraComponent.isPrimary)
        {
            renderCam = cameraComponent.camera;
        }
    }

    // Check if scene size has changed
    if (renderCam.m_width != width || renderCam.m_height != height)
    {
        renderCam.updateCameraSize(width, height);
        m_SceneWidth = width;
        m_SceneHeight = height;
    }

    // Render scene
    for (auto &&[entity, mesh, transform] : m_Registry.view<MeshComponent, TransformComponent>().each())
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
            mesh.shader.instance->UploadUniformMat4("u_ModelMatrix", transform.getMatrix());

            // Update View Matrix
            // auto viewMatrix = renderCam.getViewMatrix();
            auto viewMatrix = glm::mat4(1.0f);
            viewMatrix = glm::lookAt(glm::vec3(-0.5f, -0.2f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            mesh.shader.instance->UploadUniformMat4("u_ViewMatrix", viewMatrix);

            // Update Projection Matrix
            // auto projMatrix = renderCam.getProjMatrix();
            auto projMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
            mesh.shader.instance->UploadUniformMat4("u_ProjMatrix", projMatrix);

            // Bind the vertex array
            mesh.vao->Bind();

            // Setup Face culling and depth testing
            glEnable(GL_DEPTH_TEST);
            // glDepthFunc(GL_LESS);
            // glDisable(GL_CULL_FACE);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glDrawElements(GL_TRIANGLES, mesh.vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

            mesh.vao->Unbind();
        }
    }
}