#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform vec2 screenInvDimensions;

in vec3 lightDir;

uniform float contourShadowIntensity = 1.0;

void main()
{

    vec2 pos11 = gl_FragCoord.xy;
    vec2 pos00 = pos11 + vec2(-1,-1);
    vec2 pos01 = pos11 + vec2(-1, 0);
    vec2 pos02 = pos11 + vec2(-1, 1);
    vec2 pos10 = pos11 + vec2( 0,-1);
    vec2 pos12 = pos11 + vec2( 0, 1);
    vec2 pos20 = pos11 + vec2( 1,-1);
    vec2 pos21 = pos11 + vec2( 1, 0);
    vec2 pos22 = pos11 + vec2( 1, 1);

    vec3 normal00 = texture(normalMap,pos00 * screenInvDimensions).xyz;
    vec3 normal01 = texture(normalMap,pos01 * screenInvDimensions).xyz;
    vec3 normal02 = texture(normalMap,pos02 * screenInvDimensions).xyz;
    vec3 normal10 = texture(normalMap,pos10 * screenInvDimensions).xyz;
    vec3 normal11 = texture(normalMap,pos11 * screenInvDimensions).xyz;
    vec3 normal12 = texture(normalMap,pos12 * screenInvDimensions).xyz;
    vec3 normal20 = texture(normalMap,pos20 * screenInvDimensions).xyz;
    vec3 normal21 = texture(normalMap,pos21 * screenInvDimensions).xyz;
    vec3 normal22 = texture(normalMap,pos22 * screenInvDimensions).xyz;

    vec3 position00 = texture(positionMap,pos00 * screenInvDimensions).xyz;
    vec3 position01 = texture(positionMap,pos01 * screenInvDimensions).xyz;
    vec3 position02 = texture(positionMap,pos02 * screenInvDimensions).xyz;
    vec3 position10 = texture(positionMap,pos10 * screenInvDimensions).xyz;
    vec3 position11 = texture(positionMap,pos11 * screenInvDimensions).xyz;
    vec3 position12 = texture(positionMap,pos12 * screenInvDimensions).xyz;
    vec3 position20 = texture(positionMap,pos20 * screenInvDimensions).xyz;
    vec3 position21 = texture(positionMap,pos21 * screenInvDimensions).xyz;
    vec3 position22 = texture(positionMap,pos22 * screenInvDimensions).xyz;

    vec3 moyNormal =
        normal00 +
        normal01 +
        normal02 +
        normal10 +
        normal11 +
        normal12 +
        normal20 +
        normal21 +
        normal22;
    moyNormal /= 9.0;

    vec3 positionLaplacien =
        position11 - (
        position00 +
        position01 +
        position02 +
        position10 +
        position12 +
        position20 +
        position21 +
        position22 ) /8.0;

    vec4 color = texture(colorMap,pos11 * screenInvDimensions);

    if (dot(positionLaplacien,positionLaplacien)>0.3)
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    else if (dot(moyNormal,moyNormal)<0.95)
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    else
    {
        gl_FragColor = 0.7*vec4(1.0,1.0,1.0,1.0) + 0.3*color;

        /*gl_FragColor.xyz *= 0.9 + 0.1*dot(normalize(moyNormal),normalize(positionLaplacien));*/
        /*gl_FragColor = color;*/

        vec2 p = gl_FragCoord.xy * 0.4;
        float vx = sin(p.x+2*p.y);
        float vy = sin(p.x-2*p.y);
        /*v = int(v*5.0)/5.0;*/

        float coef = dot(normal11,normalize(lightDir));

        if (coef < 0.0)
        {
            if (vy>1.0+contourShadowIntensity*coef)
            gl_FragColor.rgb *= 0.4;
        }

        if (coef<0.3)
        {
            if (vx>1.0+contourShadowIntensity*(coef-0.3))
            gl_FragColor.rgb *= 0.4;
        }
    }
} 
