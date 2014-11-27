#version 330

in vec3 fPosition;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fBitangent;
in vec2 fTexCoord;

layout (location = 0) out vec4 outWorldPos;
layout (location = 1) out vec4 outDiffuse;
layout (location = 2) out vec4 outNormal;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

void main()
{
    outWorldPos = vec4(fPosition,1.0);
    outDiffuse  = texture(normalTexture,fTexCoord);

    vec3 bump = texture(normalTexture,fTexCoord).rgb;

    vec3 normal = normalize(fNormal);
    vec3 tangent = normalize(fTangent);
    vec3 bitangent = normalize(fBitangent);

    outNormal = vec4(normalize(normal * bump.b + tangent * bump.r + bitangent * bump.g),1.0);
} 
