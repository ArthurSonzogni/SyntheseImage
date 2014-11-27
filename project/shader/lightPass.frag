#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform mat4 projection;
uniform bool shadowsEnable;

uniform vec4 lightColor = vec4(1.0);
uniform float lightRadius = 3.0;
uniform float solidLength = 0.01;

in vec2 fTexCoord;
in vec3 fLightPosition;

const int maxStep = 100;

uniform float near = 0.1;
uniform float far = 100.0;

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
    float light = specularCoef*1.0 + diffuseCoef*0.8;


    float dist = (lightRadius - length(fLightDirection))/lightRadius;

    /*gl_FragColor = vec4(position,1.0);*/
    light *= dist;

	/* Test for screen space shadows */
	if(shadowsEnable)
	{
		float rayStepSize = 5.0/maxStep;
		bool shadowed = false;
		for(int i = 2; i<maxStep; ++i)
		{
			vec3 pos = position+ fLightDirection*i*rayStepSize;
			vec4 ppos = projection * vec4(pos,0.0);
			vec2 tpos = ppos.xy/ppos.w*0.5+0.5;
			vec4 otherPos = vec4(texture(positionMap,tpos).xyz,0.0);
			vec3 otherDir = vec3(otherPos)-position;

			float scalarProd  = abs(dot(normalize(otherDir),lightDirection));

			if (scalarProd>0.99)
			{
				shadowed = true;
				break;
			}
		}
		if(shadowed)
		{
			light = 0.0;
		}
	}
	/* end test */

    float d = length(fLightDirection.xy);

    if (d<solidLength)
        gl_FragColor = lightColor;
    else
        gl_FragColor = color * lightColor * light;
} 
