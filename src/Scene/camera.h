#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum ProjectionType
{
    Perspective,
    Orthographic
};

class Camera
{

public:
    Camera();
    Camera(float width, float height, ProjectionType projectionType = ProjectionType::Perspective);
    ~Camera() = default;

    // Initialize
    void configureCamera();

    // Update camera size
    void updateCameraSize(float width, float height);

    // Getters
    glm::mat4 getViewMatrix() const { return glm::lookAt(position, position, upVector); };
    glm::mat4 getProjMatrix();

    // Update camera
    void updateMouseMovement(float xoffset, float yoffset);
    void updateMouseZoom(float scroll);

    // Move camera
    void moveForward(float amount);
    void moveBackward(float amount);
    void moveRight(float amount);
    void moveLeft(float amount);

    // Camera Sensitivity
    float zoomSensitivity = 0.5f;
    float lookSensitivity = 0.1f;
    float moveSensitivity = 1.0f;

    // Perspective Settings
    float fieldOfView = 90.0f;
    float aspectRatio = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    // Orthographic Settings
    float zNear = -1.0f;
    float zFar = 1.0f;
    float top = 2.0f;
    float bottom = -2.0f;
    float left = -3.0f;
    float right = 3.0f;

    // Size
    float m_width = 16.0f;
    float m_height = 9.0f;

    // Camera
    glm::vec3 position = glm::vec3(-0.5f, -0.2f, -2.0f);
    glm::vec3 frontVector = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection
    ProjectionType projectionType = ProjectionType::Perspective;

    // Camera Orientation
    float yaw = 0.0f;
    float pitch = 0.0f;

private:
};

#endif