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

const int maxStep = 100;
const float rayStepSize = 5.0/maxStep;

/*void main()*/
/*{*/
    /*gl_FragColor = vec4(0,0,0,0);*/

    /*vec3 normal = normalize( texture(normalMap, fTexCoord) ).xyz;*/
    /*vec3 initialPosition = texture(positionMap, fTexCoord).xyz;*/
    /*vec3 r = reflect(vec3(0,0,-1),normal);*/

    /*for(int i = 1; i<maxStep; ++i)*/
    /*{*/
        /*vec3 pos = initialPosition + r*i*rayStepSize;*/
        /*vec4 ppos = projection * vec4(pos,0.0);*/
        /*vec2 tpos = ppos.xy/ppos.w*0.5+0.5;*/
        /*float pDepth = texture(positionMap,tpos).z;*/

        /*float diff  = pos.z - pDepth;*/

        /*const float epsilon = 0.5f;*/
        /*[>if( diff >= 0.01f && diff < 0.1)<]*/
        /*{*/
            /*gl_FragColor.rgb += 10.0*texture(colorMap,tpos).rgb/maxStep/i;*/
            /*gl_FragColor.a = 1.0;*/
            /*[>gl_FragColor.a = 1.0-10.f*length(p.xy-initialPosition.xy);<]*/
            /*[>return;<]*/
        /*}*/
    /*}*/

/*}*/
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
