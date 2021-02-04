#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexcoord;

out vec3 vCol;
out vec2 vTexcoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    vCol = aCol;
    vTexcoord = aTexcoord;
}
