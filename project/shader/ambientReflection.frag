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

const int maxStep = 15;
const float rayStepSize = 5.0/maxStep;



void main()
{
    gl_FragColor = vec4(0,0,0,0);

    vec3 normal = texture(normalMap, fTexCoord).xyz;
    vec3 position = texture(positionMap, fTexCoord).xyz;
    vec3 r = reflect(normalize(screenPos),normal);

    float specular = texture(specularMap,fTexCoord).r;
    if (specular<0.1) return;
    
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
        alpha -= best;
        // distance penalisation
        alpha -= (bestI+bestDI) / maxStep;
        // diffuse orientation
        alpha *= max(0,0.2+0.8*dot(texture(normalMap,tpos).xyz,-normal));
        // specular factor 
        alpha *= 0.5+0.5*specular;

        gl_FragColor.rgb = texture(colorMap,tpos).rgb;
        gl_FragColor.a = alpha;
        
    }
}
