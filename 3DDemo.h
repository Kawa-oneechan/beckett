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
	class Framebuffer* postFx{ nullptr };

public:
	ThreeDDemo();
	~ThreeDDemo() override;
	//bool Tick(float dt) override;
	void Draw(float dt) override;


	ThreeDDemo(ThreeDDemo&) = delete;
	ThreeDDemo operator=(ThreeDDemo) = delete;
};

#endif
