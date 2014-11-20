#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform mat4 projection;

uniform vec4 ambientColor;
uniform float param0;
uniform float param1;

in vec2 fTexCoord;

void main()
{
    float dist = param0;;
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;
    const vec3 dd[12] = vec3[]
    (
        vec3(1.0,0.0,0.0),
        vec3(0.0,1.0,0.0),
        vec3(0.0,0.0,1.0),
        vec3(-1.0,0.0,0.0),
        vec3(0.0,-1.0,0.0),
        vec3(0.0,0.0,-1.0),
        vec3(0.0,1.0,1.0),
        vec3(1.0,0.0,1.0),
        vec3(1.0,1.0,0.0),
        vec3(-0.0,1.0,1.0),
        vec3(1.0,-0.0,1.0),
        vec3(1.0,1.0,-0.0)
    );

    float occlusion = 1.0;
    for(int i = 0; i<12; ++i)
    {
        vec3 newPos = position + dist * (normal + param1*dd[i]);
        vec4 pos = projection * vec4(newPos,1.0);
        pos/=pos.w;
        vec2 texPos = pos.xy*0.5+vec2(0.5);
        /*float z = texture(positionMap,texPos).z;*/
        /*if (z < pos.z)*/
            /*occlusion *= 0.9;*/
        float dif = texture(positionMap,texPos).z - newPos.z;
        dif *= 1.0f;
        occlusion *= 1.0 - 2.f*max(0.0,dif - dif*dif*dif*dif);
        /*dif *= 0.01;*/
        /*dif *= 100;*/
        /*occlusion *= 1.0 - clamp(0.0,1.0,dif);*/
    }

    /*float occlusion = 1.0 - max(0.0,100.f*(meanZ - position.z));*/

    gl_FragColor = color * ambientColor * occlusion;

    /*gl_FragColor = vec4(occlusion,occlusion,occlusion,1.0);*/
    /*gl_FragColor = vec4(occlusion,occlusion,occlusion,1.0);*/

    /*vec3 newPos = position + dist * normal;*/
    /*vec4 pos = projection * vec4(newPos,1.0);*/
    /*pos.xyz /= pos.w;*/
    /*vec2 texPos = pos.xy * 0.5 + vec2(0.5);*/
    /*vec4 c = texture(colorMap,texPos);*/
    /*gl_FragColor = vec4(c.xyz,1.0);*/
} 
