#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection = mat4(1);
uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);

out vec3 fPosition;
out vec2 fTexCoord;
out vec3 fNormal;


void main()
{
    /*vec3 tangent = cross(vec3(0.0,0.0,1.0),normal);*/
    /*vec3 bitangent = cross(normal,tangent);*/
    /*vec3 nnormal = vec3(0.0,1.0,0.0);*/
    /*vec3 tangent = vec3(1.0,0.0,0.0);*/
    /*vec3 bitangent = vec3(0.0,0.0,1.0);*/

    vec4 worldPos = view * model * vec4(position,1.0);

    fPosition = worldPos.xyz;
    fTexCoord = texCoord;
    fNormal   = ( view * model * vec4(normal,0.0)).xyz;

    gl_Position = projection * worldPos;
}
