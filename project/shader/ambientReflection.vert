#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 fTexCoord;
out vec3 screenPos;

void main()
{
    gl_Position = vec4(position, 1.0);
    fTexCoord = position.xy*0.5+vec2(0.5);
    screenPos = position;
}
