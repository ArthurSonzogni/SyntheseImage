#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;

uniform vec4 ambientColor;

in vec2 fTexCoord;

void main()
{
    vec4 color = texture(colorMap,fTexCoord);
    gl_FragColor = color * ambientColor;
} 
