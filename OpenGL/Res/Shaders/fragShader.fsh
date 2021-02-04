#version 330 core

in vec3 vCol;
in vec2 vTexcoord;

out vec4 fragCol;

uniform vec4 auCol;
uniform float progress;
uniform sampler2D mainTex;
uniform sampler2D secTex;

void main()
{
    fragCol = mix(texture(mainTex, vTexcoord), texture(secTex, vTexcoord), progress);
}
