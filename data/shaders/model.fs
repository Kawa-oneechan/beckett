in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 FragPos;

out vec4 fragColor;

layout(binding=0) uniform sampler2DArray albedoTexture;
layout(binding=1) uniform sampler2DArray normalTexture;
layout(binding=2) uniform sampler2DArray mixTexture;
layout(binding=3) uniform sampler2DArray opacityTexture;

uniform vec3 viewPos;
uniform int layer;

#include "common.fs"
#include "lighting.fs"

void main()
{
	vec4 albedo = vec4(0.5, 0.5, 0.5, 1.0); //texture(albedoTexture, vec3(TexCoord, layer));
	vec3 normal = vec4(0.5, 0.5, 1.0, 1.0).rgb; //texture(normalTexture, vec3(TexCoord, layer)).rgb;
	vec4 mixx = vec4(1.0);
	vec4 opacity = vec4(1.0);

	vec3 norm = calcNormal(normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	for (int i = 0; i < NUMLIGHTS; i++)
		result += getLight(Lights[i], albedo.rgb, norm, viewDir, mixx.b);
	fragColor = vec4(result, opacity.r);

	//fragColor = vec4(0.5, 0.5, 0.5, 1.0); //texture(albedoTexture, TexCoord);
}
