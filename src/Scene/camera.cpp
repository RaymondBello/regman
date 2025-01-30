#include "camera.h"

Camera::Camera()
{
    configureCamera();
}

Camera::Camera(float width, float height, ProjectionType type)
{
    projectionType = type;
    updateCameraSize(width, height);
    configureCamera();
}

glm::mat4 Camera::getProjMatrix()
{
    switch (projectionType)
    {
    case ProjectionType::Perspective:
        return glm::perspective(
            glm::radians(fieldOfView), m_width / m_height,
            nearPlane, farPlane);
    case ProjectionType::Orthographic:
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    default:
        return glm::mat4(1.0f);
    };
}

void Camera::configureCamera()
{
    aspectRatio = m_width / m_height;
}

void Camera::updateCameraSize(float width, float height)
{
    m_width = width;
    m_height = height;
}

void Camera::moveForward(float amount)
{
    position += amount * moveSensitivity * frontVector;
}

void Camera::moveBackward(float amount)
{
    position -= amount * moveSensitivity * frontVector;
}

void Camera::moveRight(float amount)
{
    position += glm::normalize(glm::cross(frontVector, upVector)) * amount * moveSensitivity;
}

void Camera::moveLeft(float amount)
{
    position -= glm::normalize(glm::cross(frontVector, upVector)) * amount * moveSensitivity;
}

void Camera::updateMouseMovement(float xoffset, float yoffset)
{
    xoffset *= lookSensitivity;
    yoffset *= lookSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontVector = glm::normalize(front);
}

void Camera::updateMouseZoom(float scroll)
{
    if (fieldOfView >= 1.0f && fieldOfView <= 120.0f)
        fieldOfView -= scroll * zoomSensitivity;
    if (fieldOfView <= 1.0f)
        fieldOfView = 1.0f;
    if (fieldOfView >= 120.0f)
        fieldOfView = 120.0f;
}