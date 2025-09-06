#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <map>
#include "engine/SpriteRenderer.h"
#include "engine/TextUtils.h"
#include "engine/Utilities.h"
#include "engine/Tickable.h"
#include "engine/Types.h"

#define BECKETT_GAMENAME "Beckett Engine Template"
#define BECKETT_VERSIONJOKE "Insert Funny Here"
#define BECKETT_SCREENWIDTH 640
#define BECKETT_SCREENHEIGHT 480
#define BECKETT_DEFAULTFILTER GL_NEAREST
#define BECKETT_EXTRASAVEDIRS
//#define BECKETT_MOREVOLUME
//#define BECKETT_3DAUDIO

constexpr int MaxLights = 3;

struct Light
{
	glm::vec4 pos;
	glm::vec4 color;
};

struct CommonUniforms
{
	//Must match shaders/common.fs
	float TotalTime; //0
	float DeltaTime; //4
	glm::uvec2 ScreenRes; //8
	glm::mat4 View; //16
	glm::mat4 Projection; //80
	glm::mat4 InvView; //144
	Light Lights[MaxLights]; //208
};

namespace Beckett
{
	extern float scale;
	extern int width, height;
	extern Texture* whiteRect;
	extern CommonUniforms commonUniforms;

}

//BJTS functions that actually change the string content.
extern const std::map<std::string, Beckett::BJTSFunc> bjtsPhase1;
//BJTS functions loaded from Lua scripts.
extern std::map<std::string, std::string> bjtsPhase1X;
