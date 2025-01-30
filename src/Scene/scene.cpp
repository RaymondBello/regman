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

    default:
        printf("ERROR: Could not create object\n");
        break;
    }

    return newEntity;
}

void Scene::BeginScene()
{
    // Create entities
    auto entity = CreateEntity("Cube");
    auto shader = Shader{
        "Default Shader",
        new OpenGLShader(vShdrSrc(default), fShdrSrc(default))};
    BufferLayout layout = {
        {ShaderDataType::Float3, "aPosition"},
        {ShaderDataType::Float3, "aColor"}};
    auto meshComponent = MeshComponent(shader, cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices), layout);

    entity.AddComponent<MeshComponent>(meshComponent);
    entity.GetComponent<TransformComponent>().position = {0.0f, 0.0f, -1.7f};

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
            // glEnable(GL_DEPTH_TEST);

            // glEnable(GL_CULL_FACE);
            // glCullFace(GL_BACK);

            glDrawElements(GL_TRIANGLES, mesh.vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

            mesh.vao->Unbind();
        }
    }
}