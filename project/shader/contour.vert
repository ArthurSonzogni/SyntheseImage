#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

out vec3 lightDir;

void main()
{
    gl_Position = vec4(position, 1.0);
    lightDir = (view*model*vec4(0.0,1.0,0.0,0.0)).xyz;
}
