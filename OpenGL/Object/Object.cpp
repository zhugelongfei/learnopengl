//
//  Object.cpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/12.
//

#include "Object.hpp"
#include "../World/World.hpp"

Object::Object(const char* name, Object* parent)
:childVec(10)
{
    // init
    this->name = new std::string(name);
    
    World::GetInstance().AddObj(this);
    
    SetParent(parent);
    
    this->position = glm::vec3(0);
    this->rotation = glm::vec3(0);
}

void Object::SetParent(Object *parent)
{
    if (this->parent == parent)
        return;
    
    // remove from parent
    if (this->parent != NULL)
    {
        this->parent->RemoveChild(this);
    }
    
    // set to new parent
    this->parent = parent;
    
    // convert localPosition and localRotation
    
    // add to parent
    if (parent != NULL)
    {
        parent->AddChild(this);
    }
}

void Object::AddChild(Object *child)
{
    childVec.push_back(child);
}

void Object::RemoveChild(Object *child)
{
    for (std::vector<Object*>::iterator it = childVec.begin(); it != childVec.end(); it++)
    {
        if(*it == child)
        {
            it = childVec.erase(it);
            break;
        }
    }
}
