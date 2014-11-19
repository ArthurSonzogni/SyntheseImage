#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

uniform vec4 lightColor = vec4(1.0);
uniform float lightRadius = 3.0;

in vec2 fTexCoord;
in vec3 fLightPosition;

void main()
{
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;

    vec3 fLightDirection = fLightPosition - position;
    vec3 lightDirection = normalize(fLightDirection);
    float diffuseCoef  = max(0,dot(normal,lightDirection));
    float specularCoef = max(0,dot(reflect(normalize(position),normal),lightDirection));
    specularCoef = specularCoef*specularCoef;
    specularCoef = specularCoef*specularCoef;
    specularCoef = specularCoef*specularCoef;
    float light = specularCoef*2.0 + diffuseCoef*0.8;


    /*gl_FragColor = vec4(position,1.0);*/
    light *= (lightRadius - length(fLightDirection))/lightRadius;

    gl_FragColor = color * lightColor * light;
} 
