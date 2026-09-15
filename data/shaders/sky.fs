//Blatantly based on the sky shader from Godot,
//because there's LITERALLY NOT A SINGLE SEARCH RESULT
//for "panoramic sky shaders" that doesn't itself
//involve Godot, or is all about Minecraft title screens.

#define PI 3.14159265359
#define TAU 6.28318530717
#define SCALE 3.0

out vec4 fragColor;

#include "common.fs"

layout(binding=0) uniform sampler2D image;

const vec4 projection = vec4(0.0, 1.0, 0.0, 1.5);

float atan2(float y, float x)
{
	return mod(atan(y, x) + PI, TAU);
}

void main()
{
	vec2 uv = ((gl_FragCoord.xy / ScreenRes.xy) - vec2(0.5)) * vec2(-SCALE);

	vec3 cube = normalize(mat3(InvView) * vec3(
		(uv.x + projection.x) / projection.y,
		(uv.y + projection.z) / projection.w,
		1.0
	));

	vec2 panoCoords = vec2(atan2(cube.x, -cube.z), acos(cube.y));
	//if (panoCoords.x < 0.0) panoCoords.x += TAU;
	panoCoords /= vec2(TAU, PI);
	
	fragColor = texture(image, panoCoords);
}
