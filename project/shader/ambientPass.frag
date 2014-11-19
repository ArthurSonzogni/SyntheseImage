#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

uniform vec4 ambientColor;

in vec2 fTexCoord;

void main()
{
    vec3 position = texture(positionMap,fTexCoord).xyz;
    vec4 color = texture(colorMap,fTexCoord);
    vec3 normal = texture(normalMap,fTexCoord).xyz;

    gl_FragColor = color * ambientColor;
} 
