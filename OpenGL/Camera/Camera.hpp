//
//  Camera.hpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/1.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

enum ECameraDir
{
    Forward,
    Backward,
    Left,
    Right,
};

class Camera {
public:
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float fov;
    float yaw = -90.0f;
    float pitch;
    
    float rotateSpeed = 5.0f;
    float moveSpeed = 5.0f;
    
    float nearDis = 0.1f;
    float farDis = 100.0f;
    
public:
    Camera(float fov, glm::vec3 pos, glm::vec3 rotation);
    
    void ProcessKeyboard(ECameraDir dir, float deltaTime);
    
    void ProcessMouseMove(float deltaX, float deltaY, float deltaTime);
    
    void ProcessScroll();
    
    glm::mat4 GetProjectionMat(float width, float height);
    
    glm::mat4 GetViewMat();
};

#endif /* Camera_hpp */
