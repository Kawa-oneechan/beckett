in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 FragPos;
in vec3 WorldPos;

out vec4 fragColor;

layout(binding=0) uniform sampler2DArray albedoTexture;
layout(binding=1) uniform sampler2DArray normalTexture;
layout(binding=2) uniform sampler2DArray mixTexture;
layout(binding=3) uniform sampler2DArray opacityTexture;

uniform vec3 viewPos;
uniform int layer;
uniform mat4 model;

#include "common.fs"
#include "lighting.fs"

vec3 directLight(vec3 norm, vec3 albedo, vec3 viewDir)
{
	float ambient = 0.20;

	vec3 normal = normalize(norm);
	vec3 lightDirection = normalize(Lights[0].pos.xyz);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.50;
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDir, reflectionDirection), 0.0), 16.0);
	float specular = specAmount * specularLight;

	return (albedo.rgb * (diffuse + ambient)) * Lights[0].color.rgb;
}

float fog_exp(const float dist, const float density)
{
  return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}
float fog_exp2(const float dist, const float density)
{
  const float LOG2 = -1.442695;
  float d = density * dist;
  return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}


void main()
{
	vec4 albedo = texture(albedoTexture, vec3(TexCoord, layer));
	vec3 normal = texture(normalTexture, vec3(TexCoord, layer)).rgb;
	vec4 mixx = vec4(1.0);
	vec4 opacity = vec4(1.0);

	vec3 norm = calcNormal(normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	float fresnel = getFresnel(model, norm);
	const float fresMod = 0.5; //albedo.a in PSK

	albedo.rgb += (fresMod * fresnel);

	vec3 result;
	result = directLight(norm, albedo.rgb, viewDir);
	for (int i = 1; i < NUMLIGHTS; i++)
		result += getLight(Lights[i], albedo.rgb, norm, viewDir, mixx.b);

	/*
	float fogDistance = gl_FragCoord.z / gl_FragCoord.w;
	float fogAmount = fog_exp2(fogDistance, 0.02);
	vec3 fogColor = vec3(0.0, 0.0, 0.25);
	result = mix(result, fogColor, fogAmount);
	*/
	
	fragColor = vec4(result, opacity.r);

	//fragColor = vec4(0.5, 0.5, 0.5, 1.0); //texture(albedoTexture, TexCoord);
}
