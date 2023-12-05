#version 330 core

in vec4 Color;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D sand, grass1, grass, rock, snow, rockNormal;
uniform sampler2D SamplerTerrain;



void main()
{
   vec3 texel = vec3(texture(SamplerTerrain, TexCoords));
   FragColor = vec4(texel, 1.0);
}

/*
vec4 getTexture(inout vec3 normal, const mat3 TBN)
{
	float trans = 20.;

	vec4 sand_t = vec4(244, 231, 127, 255)/255;//texture(sand, texCoord*5.0);
	vec4 rock_t = vec4(rockColor,1.0);//texture(rock, texCoord*15.0);
	vec4 grass_t = vec4(92, 196, 66, 255)/255;//texture(grass, texCoord*5.0);

	sand_t = texture(sand, texCoord*10.0);
	sand_t.rg *= 1.3;
	rock_t = texture(rock, texCoord*vec2(1.0, 1.256).yx);
	rock_t.rgb *= vec3(2.5, 2.0, 2.0);
	grass_t = texture(grass, texCoord*12.0);//*vec4(0.0, 1.5, 0.0, 1.0);
	vec4 grass_t1 = texture(grass1, texCoord*12.0);//*
	float perlinBlendingCoeff = clamp(perlin(WorldPos.x, WorldPos.z, 2)*2.0 - 0.2, 0.0, 1.0);
	grass_t = mix(grass_t*1.3, grass_t1*0.75, perlinBlendingCoeff);
	grass_t.rgb *= 0.5;

	float grassCoverage = u_grassCoverage;//pow(u_grassCoverage, 0.33);

	
	vec4 heightColor;
	float cosV = abs(dot(normal, vec3(0.0, 1.0, 0.0)));
	float tenPercentGrass = grassCoverage - grassCoverage*0.1;
	float blendingCoeff = pow((cosV - tenPercentGrass) / (grassCoverage * 0.1), 1.0);

	if(height <= waterHeight + trans){
		heightColor = sand_t;
    }else if(height <= waterHeight + 2*trans){
		heightColor = mix(sand_t, grass_t, pow( (height - waterHeight - trans) / trans, 1.0));
    }else if(cosV > grassCoverage){
		heightColor = grass_t;
		mix(normal, vec3(0.0, 1.0, 0.0), 0.25);
    }else if(cosV > tenPercentGrass){
		heightColor = mix(rock_t , grass_t , blendingCoeff);
		normal = mix(TBN*(texture(rockNormal, texCoord*vec2(2.0, 2.5).yx).rgb*2.0 - 1.0), normal, blendingCoeff);
    }else{
		heightColor = rock_t;
		normal = TBN*(texture(rockNormal, texCoord*vec2(2.0, 2.5).yx).rgb*2.0 - 1.0);
		
	}

	return heightColor;
}*/
