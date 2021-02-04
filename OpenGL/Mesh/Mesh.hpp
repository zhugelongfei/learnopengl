//
//  Mesh.hpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/11/27.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>

#include <glad/glad.h>

class Mesh
{

public:
    Mesh();
    unsigned int id;
    
    void draw();
};

#endif /* Mesh_hpp */
