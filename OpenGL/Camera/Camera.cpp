//
//  Camera.cpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/1.
//

#include "Camera.hpp"

Camera::Camera(float fov, glm::vec3 pos, glm::vec3 rotation)
{
    this->fov = fov;
    this->pos = pos;
    this->rotation = rotation;
}

void Camera::ProcessKeyboard(ECameraDir dir, float deltaTime)
{
    if (dir == ECameraDir::Forward)
        pos += front * moveSpeed * deltaTime;
    else if (dir == ECameraDir::Backward)
        pos -= front * moveSpeed * deltaTime;
    else if (dir == ECameraDir::Left)
        pos += glm::normalize(glm::cross(worldUp, front)) * moveSpeed * deltaTime;
    else if (dir == ECameraDir::Right)
        pos -= glm::normalize(glm::cross(worldUp, front)) * moveSpeed * deltaTime;
}

void Camera::ProcessMouseMove(float deltaX, float deltaY, float deltaTime)
{
    pitch -= deltaY * deltaTime * rotateSpeed;
    yaw += deltaX * deltaTime * rotateSpeed;
    
    pitch = fmin(fmax(pitch, -80.0f), 80.0f);
    
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->front = glm::normalize(front);
}

glm::mat4 Camera::GetProjectionMat(float width, float height)
{
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(fov), width /height, nearDis, farDis);
    return projection;
}

glm::mat4 Camera::GetViewMat()
{
    return glm::lookAt(pos, pos + front, worldUp);
}
