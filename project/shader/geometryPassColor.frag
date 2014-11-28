#version 330

in vec3 fPosition;

layout (location = 0) out vec4 outWorldPos;
layout (location = 1) out vec4 outDiffuse;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outSpecular;

in vec4 fNormal;

uniform vec4 color = vec4(0.5);

void main()
{
    outWorldPos = vec4(fPosition,1.0);
    outDiffuse  = color;
    outDiffuse.rgb *= 5.0;
    outNormal   = fNormal;
    outSpecular = vec4(1.0,1.0,1.0,1.0);
} 
