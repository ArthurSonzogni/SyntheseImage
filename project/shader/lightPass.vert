#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection = mat4(1);
uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);
uniform vec3 lightPosition = vec3(0);

out vec3 fLightPosition;

void main()
{
    vec4 worldPos = view * model * vec4(position,1.0);

    fLightPosition = (view * model * vec4(lightPosition,1.0)).xyz;

    gl_Position = projection * worldPos;
}
