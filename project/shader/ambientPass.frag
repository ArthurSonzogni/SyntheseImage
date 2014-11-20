#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

uniform vec4 ambientColor;

in vec2 fTexCoord;

void main()
{
    float dist = 0.01;
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;
    float meanZ = 0.0;
    float total = 0.0;
    float dd = 0.2;
    float bias = 0.2;
    for(float dx = -1.0; dx<=1.0; dx+=dd)
    {
        for(float dy = -1.0; dy<=1.0; dy+=dd)
        {
            float z = texture(positionMap,fTexCoord + dist*vec2(dx,dy)).z;
            if (z-position.z < bias && position.z - z < bias)
            {
                meanZ+=z;
                total+=1.0;
            }
            else
            {
                meanZ+=100.0*position.z;
                total+=100.0;
                /*gl_FragColor = color * ambientColor;*/
                /*return;*/
            }
        }
    }
    meanZ = meanZ / total;

    float occlusion = 1.0 - max(0.0,20.f*(meanZ - position.z));

    gl_FragColor = color * ambientColor * occlusion;

    /*gl_FragColor = vec4(occlusion,occlusion,occlusion,1.0);*/
} 
