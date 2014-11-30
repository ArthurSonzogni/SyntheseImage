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

const int maxStep = 66;

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
    float light = pow(specularCoef,16)*texture(specularMap,texCoord).r + diffuseCoef;


    float dist = (lightRadius - length(fLightDirection))/lightRadius;

    /*gl_FragColor = vec4(position,1.0);*/
    light *= dist;

	/* Test for screen space shadows */
	if(shadowsEnable && length(fLightDirection)<=lightRadius)
	{
		float rayStepSize = 1.0/maxStep;
		float shadowed = 0;
		float bias = 0.0001;
		int shadowLimit = 10;
		for(int i = 2; i<maxStep; ++i)
		{
			// Get position to check
			vec3 pos = position+ fLightDirection*i*rayStepSize;
			vec4 ppos = projection * vec4(pos,0.0);
			// Get the texture coordinates of this position
			vec2 tpos = ppos.xy/ppos.w*0.5+0.5;
			vec2 texDir = tpos-texCoord;
			vec4 otherPos = vec4(texture(positionMap,tpos).xyz,0.0);
			vec3 otherDir = vec3(otherPos)-position;
			float halfSideTex = 100*bias * length(otherDir);
			for(float dl = -halfSideTex ; dl < halfSideTex ; dl += halfSideTex/2)
			{
				vec2 texNormal = normalize(vec2(-texDir.y,texDir.x));
				otherPos = vec4(texture(positionMap,tpos+texNormal*dl).xyz,0.0);
				otherDir = vec3(otherPos)-position;
				float scalarProd  = dot(normalize(otherDir),lightDirection);
				float halfSide = bias*length(otherDir);
				if (scalarProd>1-halfSide)
				{
					shadowed += 1.0;
					if(shadowed >= shadowLimit) break;
				}
			}
			if(shadowed >= shadowLimit) break;
		}
		light *= 1-shadowed/shadowLimit;
	}
	/* end test */

    gl_FragColor = color * lightColor * light;
    /*gl_FragColor += color * 0.5;*/
} 
