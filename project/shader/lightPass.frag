#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

in vec2 fTexCoord;
in vec3 fLightDirection;

void main()
{
    vec3 normal = texture(normalMap,fTexCoord).xyz;
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);

    vec3 lightDirection = normalize(fLightDirection);
    float diffuseCoef  = max(0,dot(normal,lightDirection));
    float specularCoef = max(0,dot(reflect(normalize(position),normal),lightDirection));
    specularCoef = specularCoef*specularCoef;
    specularCoef = specularCoef*specularCoef;
    specularCoef = specularCoef*specularCoef;
    float light = specularCoef*0.0 + diffuseCoef*0.8;

    if (diffuseCoef>0) light += 0.5;

    if (length(fLightDirection) > 1.f)
        light = 0.0f;

    gl_FragColor = color * light;
} 
