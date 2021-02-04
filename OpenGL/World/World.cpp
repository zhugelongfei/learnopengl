//
//  World.cpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/12.
//

#include "World.hpp"
#include "../Object/Object.hpp"

World* World::_instance = NULL;

void World::AddObj(Object *obj)
{
    obj->SetParent(NULL);
    totalObjVec.push_back(obj);
}

void World::DestroyObj(Object *obj)
{
    for (std::vector<Object*>::iterator it = totalObjVec.begin(); it != totalObjVec.end(); it++)
    {
        if(obj == *it)
        {
            it = totalObjVec.erase(it);
            break;
        }
    }
}
