//
//  World.hpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/12/12.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <vector>

class Object;
class World {
public:
    std::vector<Object*> totalObjVec;
    
private:
    static World* _instance;
public:
    static World& GetInstance()
    {
        if (_instance == NULL)
        {
            _instance = new World();
        }
        
        return *_instance;
    }
    
    void AddObj(Object* obj);
    void DestroyObj(Object* obj);
};

#endif /* World_hpp */
