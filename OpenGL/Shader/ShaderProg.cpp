//
//  ShaderProg.cpp
//  OpenGL
//
//  Created by 诸葛龙飞 on 2020/11/26.
//

#include "ShaderProg.hpp"

ShaderProg::ShaderProg(const char* vertFileName, const char* fragFileName)
{
    id = glCreateProgram();
    
    // load shader
    unsigned int vertShader = loadShader(GL_VERTEX_SHADER, vertFileName);
    unsigned int fragShader = loadShader(GL_FRAGMENT_SHADER, fragFileName);
    
    glAttachShader(id, vertShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);
    
    int progStatus = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &progStatus);
    if (!progStatus)
    {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader link error : " << infoLog << std::endl;
    }
    
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    
    stbi_set_flip_vertically_on_load(true);
}

void ShaderProg::use()
{
    glUseProgram(id);
}

void ShaderProg::setInt(const std::string &fieldName, int val)
{
    glUniform1i(glGetUniformLocation(id, fieldName.c_str()), val);
}

void ShaderProg::setFloat(const std::string &fieldName, float val)
{
    glUniform1f(glGetUniformLocation(id, fieldName.c_str()), val);
}

void ShaderProg::setMatrix4(const std::string &fieldName, const GLfloat* mat4)
{
    glUniformMatrix4fv(glGetUniformLocation(id, fieldName.c_str()), 1, GL_FALSE, mat4);
}

char* ShaderProg::loadFile(const char * fileName)
{
    FILE * fp = fopen(fileName, "rb");
    if(fp == NULL)
    {
        std::cout << "File open faild : " << fileName << std::endl;
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char * str = (char*)malloc(sizeof(char) * (file_size + 1));
    fread(str, file_size, sizeof(char), fp);
    str[file_size] = 0;
    fclose(fp);

    return str;
}

unsigned int ShaderProg::loadShader(int type, const char* fileName)
{
    if (fileName == NULL)
    {
        std::cout << "Shader file name is null." << std::endl;
        return 0;
    }
    
    const char* fileData = loadFile(fileName);
    
    if (!fileData)
    {
        std::cout << "Open file faild : " << fileName << std::endl;
        return 0;
    }
    
    unsigned int vertShader;
    vertShader = glCreateShader(type);
    glShaderSource(vertShader, 1, &fileData, NULL);
    int compileStatus = 0;
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
    if(!compileStatus)
    {
        char infoLog[1024];
        glGetShaderInfoLog(vertShader, 1024, NULL, infoLog);
        std::cout << "Shader compile error : \n" << infoLog << "\n" << fileData << std::endl;
    }
    
    delete [] fileData;
    
    return vertShader;
}


void ShaderProg::setTexture(const char* texPath, const char* fieldName, int idx)
{
    // 1: use shader program.
    use();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // 2: set texture idx and active it.
    setInt(fieldName, idx);
    glActiveTexture(GL_TEXTURE0 + idx);
    
    // 3: generate texture.
    unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // 4: loda file from disk
    int width, height, nColChannels;
    unsigned char* texData = stbi_load(texPath, &width, &height, &nColChannels, 0);

    if (texData)
    {
        // gen texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Texture load faild." << std::endl;
    }
    stbi_image_free(texData);
}

