#ifndef SCENE_H
#define SCENE_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "components.h"

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    void BeginScene();
    void RenderScene(float width, float height);
    void EndScene();

    Entity CreateEntity(std::string name = "Entity");
    Entity CreateRawEntity(std::string name = "Entity");
    Entity AddObject(ObjectType type);

    // TEMP
    entt::registry &Reg() { return m_Registry; }

    // GLfloat cubeVertices[144] = {
    //     // front
    //     -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0 (red)
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f,  // 1 (orange)
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,   // 2 (yellow)
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 3 (green)

    //     // top
    //     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 4 (blue)
    //     0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // 5 (indigo)
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,   // 6 (violet)
    //     -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,  // 7 (gray)

    //     // left
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // 8 (cyan)
    //     -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f,  // 9 (pink)
    //     -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f,   // 10 (light green)
    //     -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f,  // 11 (light blue)

    //     // right
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, // 12 (dark pink)
    //     0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  // 13 (light yellow)
    //     0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // 14 (dark purple)
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f,  // 15 (orange yellow)

    //     // back
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, // 16 (light blue)
    //     0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  // 17 (yellow green)
    //     0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f,   // 18 (dark green)
    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f,  // 19 (dark pink)

    //     // bottom
    //     -0.5f, -0.5f, 0.5f, 0.3f, 0.3f, 0.3f, // 20
    //     0.5f, -0.5f, 0.5f, 0.3f, 0.3f, 0.3f,  // 21
    //     0.5f, -0.5f, -0.5f, 0.3f, 0.3f, 0.3f, // 22
    //     -0.5f, -0.5f, -0.5f, 0.3f, 0.3f, 0.3f // 23
    // };

    // GLuint cubeIndices[36] = {
    //     // front
    //     0, 1, 2, // first triangle
    //     2, 3, 0, // second triangle

    //     // top
    //     4, 5, 6, // first triangle
    //     6, 7, 4, // second triangle

    //     // left
    //     8, 9, 10,  // first triangle
    //     10, 11, 8, // second triangle

    //     // right
    //     14, 13, 12, // 12, 13, 14, // first triangle
    //     12, 15, 14, // 14, 15, 12, // second triangle

    //     // back
    //     18, 17, 16, // 16, 17, 18, // first triangle
    //     16, 19, 18, // 18, 19, 16, // second triangle

    //     // bottom
    //     20, 21, 22, // first triangle
    //     22, 23, 20  // second triangle
    // };
    // GLuint cubeIndices[36] = {
    //     0, 1, 2, 2, 3, 0,       // Front
    //     4, 5, 6, 6, 7, 4,       // Top
    //     8, 9, 10, 10, 11, 8,    // Left
    //     12, 13, 14, 14, 15, 12, // Right
    //     16, 17, 18, 18, 19, 16, // Back
    //     20, 21, 22, 22, 23, 20  // Bottom
    // };

    // Plane indices
    GLfloat planeVertices[24] = {
        // front
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0 (red)
        0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f,  // 1 (orange)
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,   // 2 (yellow)
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 3 (green)
    };

    GLuint planeIndices[6] = {0, 1, 2, 2, 3, 0};

    GLfloat cubeVertices[30] = {
    // Apex (Top point) - White
     0.0f,  0.5f,  0.0f,  1.0f, 1.0f, 1.0f, 
    // Base (Square at y = -0.5f)
    // Bottom-left - Red
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  
    // Bottom-right - Green
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  
    // Top-right - Blue
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  
    // Top-left - Yellow
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  
    };

    // Indices for drawing triangles
    GLuint cubeIndices[18] = {
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
    // Camera
    Camera m_ActiveCamera;

private:
    entt::registry m_Registry;

    // Size of scene
    float m_SceneWidth;
    float m_SceneHeight;
};

#endif