#pragma once
#ifndef BECKETT_NO3DMODELS
#include "engine/Tickable.h"
#include "engine/Model.h"
#include "Camera.h"

class TrainScene : public Tickable
{
private:
	Model model{ "examples/train/train.fbx" };
	float bumpTimer{ 0.0f };
	//Framebuffer* postFx{ nullptr };

	ModelP bob{ nullptr };
	ModelP boob{ nullptr };

public:
	TrainScene();
	~TrainScene() override;

	TrainScene(const TrainScene&) = delete;
	TrainScene &operator=(const TrainScene&) = delete;

	bool Tick(float dt) override;
	void Draw(float dt) override;
};
#endif
