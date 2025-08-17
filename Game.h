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

extern float scale;
extern int width, height;

#define GAMENAME "Beckett Engine Template"
#define VERSIONJOKE "Insert Funny Here"
#define SCREENWIDTH 640
#define SCREENHEIGHT 480

#define DEFAULTFILTER GL_NEAREST
#define BECKETT_EXTRASAVEDIRS

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
extern CommonUniforms commonUniforms;

//BJTS functions that actually change the string content.
extern const std::map<std::string, BJTSFunc> bjtsPhase1;
//BJTS functions loaded from Lua scripts.
extern std::map<std::string, std::string> bjtsPhase1X;
