#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection = mat4(1);
uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);
uniform vec3 lightPosition = vec3(0);
uniform float lightRadius = 3.0;
uniform vec4 lightColor = vec4(1.0);

out vec2 fTexCoord;
out vec3 fLightPosition;
out float fRadius;

void main()
{
    vec4 worldPos = view * model * vec4(position,1.0);

    fLightPosition = (view * model * vec4(lightPosition,1.0)).xyz;
    fRadius = lightRadius;

    gl_Position = projection * worldPos;

    fTexCoord = (gl_Position.xy)/gl_Position.w*0.5+vec2(0.5);
}
