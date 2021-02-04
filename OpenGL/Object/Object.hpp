//
//  Object.hpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/12.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Object
{
public:
    std::string* name = NULL;
    glm::vec3 position;
    glm::vec3 rotation;

    glm::vec3 localPosition;
    glm::vec3 localRotation;
    
    Object* parent = NULL;
    
    std::vector<Object*> childVec;
    
private:
    void AddChild(Object* child);
    void RemoveChild(Object* child);
    
public:
    Object(const char* name, Object* parent = NULL);
    
    void Rotate(float angle, glm::vec3 axis);
    void Translate(glm::vec3 offsetPos);
    
    void SetParent(Object* parent);
};

#endif /* Object_hpp */
