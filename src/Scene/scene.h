#ifndef SCENE_H
#define SCENE_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "components.h"
#include "editor.h"

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    void BeginScene();
    void BeginEditor();
    void RenderScene(float width, float height);
    void EndScene();

    Entity CreateEntity(std::string name = "Entity");
    Entity CreateRawEntity(std::string name = "Entity");
    Entity AddObject(ObjectType type);

    // TEMP
    entt::registry &Reg() { return m_Registry; }

    GLfloat cubeVertices[144] = {
        // Front face
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f,  // Bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,   // Top-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Top-left

        // Back face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, // Bottom-left
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  // Bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f,   // Top-right
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f,  // Top-left

        // Left face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom-left
        -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f,  // Top-left
        -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f,   // Top-right
        -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f,  // Bottom-right

        // Right face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  // Top-left
        0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // Top-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f,  // Bottom-right

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // Bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,   // Top-right
        -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,  // Top-left

        // Bottom face (now with proper colors)
        -0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.8f, // Bottom-left
        0.5f, -0.5f, -0.5f, 0.7f, 0.7f, 0.7f,  // Bottom-right
        0.5f, -0.5f, 0.5f, 0.6f, 0.6f, 0.6f,   // Top-right
        -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f   // Top-left
    };

    GLuint cubeIndices[36] = {
        // front
        0, 1, 2,
        2, 3, 0,

        // top
        4, 5, 6,
        6, 7, 4,

        // left
        8, 9, 10,
        10, 11, 8,

        // right (FIXED winding order)
        12, 13, 14,
        14, 15, 12,

        // back (FIXED winding order)
        16, 17, 18,
        18, 19, 16,

        // bottom
        20, 21, 22,
        22, 23, 20};

    // Quad indices
    GLfloat quadVertices[32] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0 (red)
        -0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,  // 1 (orange)
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // 2 (yellow)
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 3 (green)
    };

    GLuint quadIndices[6] = {0, 2, 1, 0, 3, 2};

    GLfloat pyramidVertices[30] = {
        // Apex (Top point) - White
        0.0f,
        0.5f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        // Base (Square at y = -0.5f)
        // Bottom-left - Red
        -0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        0.0f,
        0.0f,
        // Bottom-right - Green
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.0f,
        // Top-right - Blue
        0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        1.0f,
        // Top-left - Yellow
        -0.5f,
        -0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.0f,
    };

    // Indices for drawing triangles
    GLuint pyramidIndices[18] = {
        // Front Face
        0, 1, 2,
        // Right Face
        0, 2, 3,
        // Back Face
        0, 3, 4,
        // Left Face
        0, 4, 1,
        // Base (Two Triangles)
        1, 2, 3,
        1, 3, 4};

    GLfloat texPyramidVertices[40] =
        { //     COORDINATES     /        COLORS      /   TexCoord  //
            -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
            0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f,
            0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f};

    // Indices for vertices order
    GLuint texPyramidIndices[18] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4};
    // Camera
    Camera m_ActiveCamera;
    TextEditor editor;
    char *fileToEdit = "assets/scripts/controller.lua";

private:
    entt::registry m_Registry;

    // Size of scene
    float m_SceneWidth;
    float m_SceneHeight;
};

#endif