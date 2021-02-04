//
//  ShaderProg.hpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/11/26.
//

#ifndef ShaderProg_hpp
#define ShaderProg_hpp

#include <stdio.h>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include "../libs/stb_image.h"

class ShaderProg
{
private:
    char* loadFile(const char* fileName);
    unsigned int loadShader(int type, const char* fileName);
    
public:
    unsigned int id;
    
    ShaderProg(const char* vertFileName, const char* fragFileName);
    
    // use program
    void use();
    
    // texture
    void setTexture(const char* texPath, const char* fieldName, int idx);
    
    // uniform
    void setInt(const std::string &fieldName, int val);
    void setFloat(const std::string &fieldName, float val);
    void setMatrix4(const std::string &fieldName, const GLfloat* mat4);
};

#endif /* ShaderProg_hpp */
