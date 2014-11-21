#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform mat4 projection;

uniform vec4 ambientColor;
uniform float param0;
uniform float param1;

in vec2 fTexCoord;
in vec3 screenPos;

const int maxStep = 20;
const float rayStepSize = 0.1/maxStep;

void main()
{
    vec3 normal = normalize( texture(normalMap, fTexCoord) ).xyz;
    float currDepth = texture(positionMap, fTexCoord).z;

    vec3 reflected = reflect(vec3(0,0,-1),normal);
    vec4 r = projection * vec4(reflected,0.0);
    r.xy/= r.w;

    gl_FragColor = vec4(0,0,0,0);

    float stepSize = rayStepSize; 
    vec3 reflectionVector = vec3(r.xyz) * rayStepSize;
    vec3 initialPosition = vec3(fTexCoord,currDepth);
    vec3 position = initialPosition;

    for(int i = 1; i<maxStep; ++i)
    {
        position += reflectionVector;
        float pDepth = texture(positionMap, position.xy).z;


        if( position.z < pDepth + 0.05f &&
            position.z > pDepth - 0.5f)
        {
            gl_FragColor.rgb = texture(colorMap,position.xy).rgb;
            gl_FragColor.a = 1.0-10.f*length(position.xy-initialPosition.xy);
            return;
        }
    }

}


/*void main()*/
/*{*/
/*float dist = 0.1;*/
/*vec3 position = texture(positionMap,fTexCoord).xyz;*/
/*vec4 color = texture(colorMap,fTexCoord);*/
/*vec3 normal = texture(normalMap,fTexCoord).xyz;*/
/*vec3 reflection = reflect(vec3(0.0,0.0,1.0),normal);*/

/*for(int i = 0; i < 10.0; ++i)*/
/*{*/
/*vec3 newPos = position + reflection*i*10.0;*/
/*vec4 pos = projection * vec4(newPos,1.0);*/
/*pos.xy/=pos.w;*/
/*vec2 texPos = pos.xy*0.5+vec2(0.5);*/
/*vec3 realPos = texture(positionMap,texPos).xyz;*/
/*const float bias = 3.0;*/
/*if (realPos.z + bias > newPos.z && newPos.z + bias>realPos.z)*/
/*{*/
/*vec3 c = texture(colorMap,texPos).xyz;*/
/*gl_FragColor = vec4(c*0.8,1.0);*/
/*return;*/
/*}*/
/*}*/
/*gl_FragColor = vec4(0.0);*/
/*} */
