#version 330

in vec3 fPosition;
in vec2 fTexCoord;

in mat3 repere;

layout (location = 0) out vec4 outWorldPos;
layout (location = 1) out vec4 outDiffuse;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec4 outSpecular;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

uniform mat4 model = mat4(1);
uniform mat4 view = mat4(1);

void main()
{
    outWorldPos = vec4(fPosition,1.0);
    outDiffuse  = texture(colorTexture,fTexCoord);

    vec3 bump = texture(normalTexture,fTexCoord).rgb;
    /*bump.r = - bump.r;*/
    /*bump.g = - bump.g;*/
    /*bump.b = 1.0;*/
    /*bump.x = 0.0;*/
    /*bump.y = 0.0;*/
    /*bump.z = 1.0;*/

    outNormal = vec4(normalize(mat3(view * model) * bump.gbr),0.0);

    outSpecular = texture(specularTexture,fTexCoord).rrrr;
} 
