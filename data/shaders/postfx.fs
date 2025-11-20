in vec2 TexCoords;
flat in int index;

out vec4 fragColor;

#include "common.fs"

layout(binding=0) uniform sampler2D image;
uniform vec4 spriteColor[200];
uniform vec4 sourceRect[200];
uniform bool flipX[200], flipY[200];

vec3 PBRNeutralToneMapping(vec3 color)
{
	const float startCompression = 0.8 - 0.04;
	const float desaturation = 0.15;

	float x = min(color.r, min(color.g, color.b));
	float offset = x < 0.08 ? x - 6.25 * x * x : 0.04;
	color -= offset;

	float peak = max(color.r, max(color.g, color.b));
	if (peak < startCompression) return color;

	const float d = 1. - startCompression;
	float newPeak = 1. - d * d / (peak + d - startCompression);
	color *= newPeak / peak;

	float g = 1. - 1. / (desaturation * (peak - newPeak) + 1.);
	return mix(color, newPeak * vec3(1, 1, 1), g);
}

vec4 lookup(vec2 coords)
{
	vec3 x = texture(image, coords).rgb;
	x = PBRNeutralToneMapping(x);
	return vec4(x, 1.0);
}

void main()
{
	fragColor = lookup(TexCoords);
}
