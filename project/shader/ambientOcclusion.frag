#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform mat4 projection;

uniform vec4 ambientColor;
uniform float occlusionDistance;
uniform float occlusionIntensity;

in vec2 fTexCoord;

void main()
{
    float dist = occlusionDistance;
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;
    const vec3 dd[12] = vec3[]
    (
        vec3(+1.0,+0.0,+0.0),
        vec3(-1.0,+0.0,+0.0),
        vec3(+0.0,+1.0,+0.0),
        vec3(+0.0,-1.0,+0.0),
        vec3(+0.0,+0.0,+1.0),
        vec3(+0.0,+0.0,-1.0),
        vec3(+1.0,+1.0,+1.0),
        vec3(-1.0,-1.0,-1.0),
        vec3(+1.0,-1.0,+1.0),
        vec3(-1.0,+1.0,-1.0),
        vec3(+1.0,+1.0,-1.0),
        vec3(-1.0,-1.0,+1.0)
    );

    float occlusion = 0.0;
    for(int i = 0; i<12; ++i)
    {
        vec3 newPos = position + dist * (normal + 0.6*dd[i]);
        vec4 pos = projection * vec4(newPos,1.0);
        pos/=pos.w;
        vec2 texPos = pos.xy*0.5+vec2(0.5);
        vec3 realPos = texture(positionMap,texPos).xyz;
        float dif = realPos.z - newPos.z;
        dif *= 1.5f;
        occlusion += max(0.0,dif - dif*dif*dif*dif)/(1.0+length(realPos-newPos));
    }
    occlusion *= occlusionIntensity;
    gl_FragColor = vec4(occlusion,occlusion,occlusion,0.0);
} 
