//---Common---

layout (std140) uniform CommonData
{
	float TotalTime; //0
	float DeltaTime; //4
	uvec2 ScreenRes; //8
	mat4 View; //16
	mat4 Projection; //?
	mat4 InvView; //?
};

//------------
