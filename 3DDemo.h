#pragma once
#ifndef BECKETT_NO3DMODELS
#include "engine/Tickable.h"
#include "engine/Model.h"
#include "Camera.h"

class MapScene : public Tickable
{
private:
	Model model{ "examples/3dscene/scene.fbx" };

public:
	MapScene();
	//bool Tick(float dt) override;
	void Draw(float dt) override;
};


#endif
