//---Common---

#define NUMLIGHTS 3

struct light
{
	vec4 pos;
	vec4 color;
};

layout (std140) uniform CommonData
{
	float TotalTime;
	float DeltaTime;
	uvec2 ScreenRes;
	mat4 View;
	mat4 Projection;
	mat4 InvView;
	light Lights[NUMLIGHTS];
};

//------------
