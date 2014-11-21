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
const float rayStepSize = 4.0/maxStep;

void main()
{
    vec3 normal = normalize( texture(normalMap, fTexCoord) ).xyz;
    vec3 initialPosition = texture(positionMap, fTexCoord).xyz;
    vec3 reflected = reflect(vec3(0,0,-1),normal);

    gl_FragColor = vec4(0,0,0,0);

    for(int i = maxStep-5; i<maxStep; ++i)
    {
        vec3 position = initialPosition + i*reflected*rayStepSize;
        vec4 p = projection * vec4(position,1.0);
        vec2 tcoord = p.xy/p.w*0.5+0.5;
        float pDepth = texture(positionMap,tcoord).z;


        /*if( position.z < pDepth - 0.4f &&*/
            /*position.z > pDepth - 3.0f)*/
        {
            gl_FragColor.rgb += texture(colorMap,tcoord).rgb*(maxStep-i)/maxStep/5;
            gl_FragColor.a = 1.0;
            /*gl_FragColor.a = 1.0-10.f*length(p.xy-initialPosition.xy);*/
            /*return;*/
        }
    }

}
/*void main()*/
/*{*/
    /*vec3 normal = normalize( texture(normalMap, fTexCoord) ).xyz;*/
    /*float currDepth = texture(positionMap, fTexCoord).z;*/

    /*vec3 reflected = reflect(vec3(0,0,-1),normal);*/
    /*vec4 r = projection * vec4(reflected,0.0);*/
    /*r.xy/= r.w;*/

    /*gl_FragColor = vec4(0,0,0,0);*/

    /*float stepSize = rayStepSize; */
    /*vec3 reflectionVector = vec3(r.xyz) * rayStepSize;*/
    /*vec3 initialPosition = vec3(fTexCoord,currDepth);*/
    /*vec3 position = initialPosition;*/

    /*for(int i = 1; i<maxStep; ++i)*/
    /*{*/
        /*position += reflectionVector;*/
        /*float pDepth = texture(positionMap, position.xy).z;*/


        /*if( position.z < pDepth + 0.05f &&*/
            /*position.z > pDepth - 0.5f)*/
        /*{*/
            /*gl_FragColor.rgb = texture(colorMap,position.xy).rgb;*/
            /*gl_FragColor.a = 1.0-10.f*length(position.xy-initialPosition.xy);*/
            /*return;*/
        /*}*/
    /*}*/

/*}*/
