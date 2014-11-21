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
    float dist = 0.1;
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;
    const vec3 dd[24] = vec3[]
    (
        vec3(-0.27626228950744,
        -0.33423055956803,
        -0.56205942275098),
        vec3(0.30192993828185,
        -0.71445902144278,
        0.40652998043528),
        vec3(0.092947838405589,
        -0.65584521538384,
        0.558154312688),
        vec3(-0.88091095252005,
        0.22916512434798,
        -0.31634078329259),
        vec3(-0.95389500910132,
        -0.23526124806854,
        -0.26145279838771),
        vec3(0.92241729885452,
        -0.68444916870652,
        0.04059049442438),
        vec3(-0.64053637703906,
        -0.96467773614669,
        0.58513053207897),
        vec3(-0.75863460905786,
        0.19086737986229,
        -0.68977477666446),
        vec3(-0.64775406925369,
        -0.18945159073428,
        -0.8256464553185),
        vec3(-0.59158426876719,
        -0.93721927233842,
        -0.91822844646789),
        vec3(0.98327987174656,
        -0.80499164285371,
        0.13700845424878),
        vec3(-0.008441547401455,
        -0.085258406160985,
        0.57313926870615),
        vec3(0.96937960664247,
        -0.33502198445379,
        -0.71611576141609),
        vec3(-0.36357780423182,
        -0.59178763050204,
        0.84977286302008),
        vec3(-0.13587775041157,
        -0.48830243083104,
        0.40038318904135),
        vec3(0.1041448242516,
        -0.39743830980614,
        0.70405101203548),
        vec3(-0.59666757918739,
        0.8655664305508,
        0.27884285304641),
        vec3(-0.66812471750571,
        0.81787803946895,
        -0.37498858355684),
        vec3(0.44691108886474,
        -0.17700814976218,
        0.45659927625982),
        vec3(-0.44756319068724,
        0.025839998864494,
        0.9342048251695),
        vec3(0.13846478664245,
        -0.92284310884906,
        -0.66522017571387),
        vec3(-0.39246562374405,
        0.70404072837161,
        0.23438780626021),
        vec3(0.31044783876764,
        -0.75311474304326,
        -0.52742125165063),
        vec3(0.39159462852012,
        -0.57219929693835,
        0.21485454366303)
    );

    float occlusion = 0.0;
    for(int i = 0; i<12; ++i)
    {
        vec3 newPos = position + dist * (normal + 0.8*dd[i]);
        vec4 pos = projection * vec4(newPos,1.0);
        pos/=pos.w;
        vec2 texPos = pos.xy*0.5+vec2(0.5);
        vec3 realPos = texture(positionMap,texPos).xyz;
        float dif = realPos.z - newPos.z;
        dif *= 1.5f;
        occlusion += 0.8f*max(0.0,dif - dif*dif*dif*dif)/(1.0+length(realPos-newPos));
    }
    occlusion = min(occlusion,0.8);
    gl_FragColor = vec4(occlusion,occlusion,occlusion,0.0);
} 
