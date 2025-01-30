#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "opengl_buffer.h"
#include "opengl_shader.h"
#include "camera.h"

struct TagComponent
{
    TagComponent(const std::string &tag = std::string("")) : tag{tag}
    {
    }

    std::string tag;
};

struct TransformComponent
{
    // Constructor that allows to set position and rotation, with default rotation value.
    TransformComponent(const glm::vec3 &position = glm::vec3{0.0f, 0.0f, 0.0f},
                       const glm::vec3 &rotation = glm::vec3{0.0f, 0.0f, 0.0f},
                       const glm::vec3 &scale = glm::vec3{1.0f, 1.0f, 1.0f}) : position{position}, // default position
                                                                               rotation{rotation}, // default rotation in degrees
                                                                               scale{scale}        // default scale
    {
    }

    // Get the transformation matrix
    glm::mat4 getMatrix()
    {
        auto transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate(transformMatrix, position);
        transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        transformMatrix = glm::scale(transformMatrix, scale);
        return transformMatrix;
    }

    // Position of the entity in 3D space.
    glm::vec3 position;

    // Rotation of the entity in 3D space around each axis in degrees.
    glm::vec3 rotation;

    // Scale of the entity in 3D space.
    glm::vec3 scale;
};

struct CameraComponent
{
    CameraComponent() {};
    ~CameraComponent() = default;
    CameraComponent(float width, float height) { camera = Camera(width, height); };

    Camera camera;
    bool isPrimary = false;
};

struct Shader
{
    std::string name;
    OpenGLShader *instance = nullptr;
};

struct Mesh
{
    std::vector<GLuint> indices;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textureCoords;
};

struct MeshComponent
{
    MeshComponent() = default;
    MeshComponent(Shader shader, GLfloat vertices[], size_t verticesSize, GLuint indices[], size_t indicesSize, BufferLayout bufferLayout) : shader{shader}
    {
        vao = std::make_shared<OpenGLVertexArray>();
        vbo = std::make_shared<OpenGLVertexBuffer>(vertices, verticesSize);
        ibo = std::make_shared<OpenGLIndexBuffer>(indices, indicesSize / sizeof(GLuint));
        layout = bufferLayout;

        vbo->SetLayout(layout);
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);

        if (shader.instance != nullptr)
        {
            // Bind the shader and initialize the uniforms
            shader.instance->Bind();
            shader.instance->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));
            shader.instance->Unbind();
        }
    };

    Shader shader;
    BufferLayout layout;

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<OpenGLVertexArray> vao;
    std::shared_ptr<OpenGLVertexBuffer> vbo;
    std::shared_ptr<OpenGLIndexBuffer> ibo;
};

#endif