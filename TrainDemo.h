#pragma once
#ifndef BECKETT_NO3DMODELS
#include "engine/Tickable.h"
#include "engine/Model.h"
#include "Camera.h"

class TrainDemo : public Tickable
{
private:
	Model model{ "examples/train/train.fbx" };
	float bumpTimer{ 0.0f };
	//Framebuffer* postFx{ nullptr };

	ModelP bob{ nullptr };
	ModelP boob{ nullptr };

public:
	TrainDemo();
	~TrainDemo() override;

	TrainDemo(const TrainDemo&) = delete;
	TrainDemo &operator=(const TrainDemo&) = delete;

	bool Tick(float dt) override;
	void Draw(float dt) override;
};
#endif
