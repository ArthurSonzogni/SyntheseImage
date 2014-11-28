#version 330

uniform sampler2D positionMap;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform mat4 projection;
uniform bool shadowsEnable;

uniform vec4 lightColor = vec4(1.0);
uniform float lightRadius = 3.0;
uniform float solidLength = 0.01;

in vec3 fLightPosition;
in vec2 fTexCoord;

const int maxStep = 100;

uniform float near = 0.1;
uniform float far = 100.0;
uniform vec2 screenInvDimensions = vec2(1.0/640.0,1.0/480.0);

void main()
{
    vec2 texCoord = gl_FragCoord.xy * screenInvDimensions;
    vec3 position = texture(positionMap,texCoord).xyz;
    vec4 color = texture(colorMap,texCoord);
    vec3 normal = texture(normalMap,texCoord).xyz;

    vec3 fLightDirection = fLightPosition - position;
    vec3 lightDirection = normalize(fLightDirection);
    float diffuseCoef  = max(0,dot(normal,lightDirection));
    float specularCoef = max(0,dot(reflect(normalize(position),normal),lightDirection));
    float light = specularCoef*1.0 + pow(diffuseCoef,8)*texture(specularMap,texCoord).r;


    float dist = (lightRadius - length(fLightDirection))/lightRadius;

    /*gl_FragColor = vec4(position,1.0);*/
    light *= dist;

	/* Test for screen space shadows */
    /*
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
    */
	/* end test */

    gl_FragColor = color * lightColor * light;
    /*gl_FragColor += color * 0.5;*/
} 
