#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform mat4 projection;

uniform vec4 ambientColor;
uniform float param0;
uniform float param1;

in vec2 fTexCoord;
in vec3 screenPos;

const int maxStep = 10;
const float rayStepSize = 5.0/maxStep;

uniform float near = 0.1;
uniform float far = 100.0;

float linearizeDepth(float depth)
{
    return (near * depth) / ( far - depth * (far - near) );
}


void main()
{
    gl_FragColor = vec4(0,0,0,0);

    vec3 normal = texture(normalMap, fTexCoord).xyz;
    vec3 position = texture(positionMap, fTexCoord).xyz;
    vec3 r = reflect(normalize(screenPos),normal);
    
    float best = 1000.0;
    int bestI = 0;

    // research 
    for(int i = 2; i<maxStep; ++i)
    {
        vec3 pos = position+ r*i*rayStepSize;
        vec4 ppos = projection * vec4(pos,0.0);
        vec2 tpos = ppos.xy/ppos.w*0.5+0.5;
        float pDepth = texture(positionMap,tpos).z;

        float diff  = abs(pos.z - pDepth);

        if (diff<best)
        {
            best = max(diff,-diff);
            bestI = i;
            if (best<0.01) break;
        }
    }

    float bestDI = 0.0;

    // refinement
    for(float di = -0.9; di<0.9; di+=0.1)
    {
        vec3 pos = position+ r*(bestI+di)*rayStepSize;
        vec4 ppos = projection * vec4(pos,0.0);
        vec2 tpos = ppos.xy/ppos.w*0.5+0.5;
        float pDepth = texture(positionMap,tpos).z;

        float diff  = pos.z - pDepth;

        if (diff<best && -diff<best)
        {
            best = max(diff,-diff);
            bestDI = di;
            if (best<0.01) break;
        }
    }

    // final
    {
        vec3 pos = position+ r*(bestI+bestDI)*rayStepSize;
        vec4 ppos = projection * vec4(pos,0.0);
        vec2 tpos = ppos.xy/ppos.w*0.5+0.5;
        float alpha = 1.0;
        // imprecision penalisation
        alpha -= 1.2 * best;
        // distance penalisation
        alpha -= (bestI+bestDI) / maxStep;
        // diffuse orientation
        alpha *= max(0,dot(texture(normalMap,tpos).xyz,-normal));
        // specular factor 
        alpha *= texture(specularMap,fTexCoord).r;

        gl_FragColor.rgb = texture(colorMap,tpos).rgb;
        gl_FragColor.a = alpha;
        
    }
    /*gl_FragColor.rgb = texture(specularMap,fTexCoord).rrr;*/
    /*gl_FragColor.a = 1.0;*/
    /*gl_FragColor.a *= texture(specularMap,fTexCoord).r;*/

}
/*void main()*/
/*{*/
    /*vec3 normal = normalize( texture(normalMap, fTexCoord) ).xyz;*/
    /*float currDepth = texture(positionMap, fTexCoord).z;*/

    /*vec3 reflected = reflect(vec3(0,0,-1),normal);*/
    /*vec4 r = projection * vec4(reflected,0.0);*/
    /*r.xy/= r.w;*/

    /*gl_FragColor = vec4(0,0,0,0);*/

    /*float stepSize = rayStepSize;*/
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

