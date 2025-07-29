#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "engine/SpriteRenderer.h"
#include "engine/TextUtils.h"
#include "engine/Utilities.h"
#include "engine/Tickable.h"

extern float scale;
extern int width, height;

#define GAMENAME "Beckett Engine Template"
#define VERSIONJOKE "Insert Funny Here"
#define SCREENWIDTH 320
#define SCREENHEIGHT 240

#define DEFAULTFILTER GL_NEAREST

struct CommonUniforms
{
	//Must match shaders/common.fs
	float TotalTime; //0
	float DeltaTime; //4
	glm::uvec2 ScreenRes; //24
	glm::mat4 View; //32
	glm::mat4 Projection; //96
	glm::mat4 InvView; //160
};
extern CommonUniforms commonUniforms;
