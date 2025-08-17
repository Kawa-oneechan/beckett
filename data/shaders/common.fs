//---Common---

#define NUMLIGHTS 3

struct light
{
	vec4 pos;
	vec4 color;
};

layout (std140) uniform CommonData
{
	float TotalTime; //0
	float DeltaTime; //4
	uvec2 ScreenRes; //8
	mat4 View; //16
	mat4 Projection; //80
	mat4 InvView; //144
	light Lights[NUMLIGHTS]; //208
};

//------------
