#pragma once
#ifndef BECKETT_NO3DMODELS
#include "engine/Tickable.h"
#include "engine/Model.h"
#include "Camera.h"

class ThreeDDemo : public Tickable
{
private:
	Model model{ "examples/3dscene/scene.fbx" };
	//Model model{ "examples/3dscene/teapot without armature.fbx" };

public:
	ThreeDDemo();
	//bool Tick(float dt) override;
	void Draw(float dt) override;
};

#endif
