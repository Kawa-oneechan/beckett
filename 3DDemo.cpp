#ifndef BECKETT_NO3DMODELS
#include "3DDemo.h"
#include "DemoUI.h"

extern float fieldOfView;
extern void RecalcProjections();

MapScene::MapScene()
{
	ID = "Map Scene";

	//commonUniforms.Lights[0].color = glm::vec4(1.0, 1.0, 1.0, 0.5);
	//commonUniforms.Lights[0].pos = glm::vec4(20, 15, 0, 0);
	commonUniforms.Lights[0].color = glm::vec4(0);
	commonUniforms.Lights[0].pos = glm::vec4(0);
	commonUniforms.Lights[1].pos = model.Lights[0].Position;
	commonUniforms.Lights[1].color = model.Lights[0].Color;
	commonUniforms.Lights[2].color = glm::vec4(0);
	commonUniforms.Lights[2].pos = glm::vec4(0);

	//Maybe figure out how to get this from a UfbxMisc camera?
	fieldOfView = 22.5f;
	RecalcProjections();

	MainCamera->FirstPerson(true);
	MainCamera->Target(model.Cameras[0].Position);
	MainCamera->Angles(model.Cameras[0].Direction);

	MainCamera->Distance(0);
}

/*
bool MapScene::Tick(float dt)
{
	//commonUniforms.Lights[0].pos.x = 20 * glm::cos(commonUniforms.TotalTime * 0.5f);
	//commonUniforms.Lights[0].pos.y = 20 * glm::sin(commonUniforms.TotalTime * 0.5f);
	return Tickable::Tick(dt);
}
*/

void MapScene::Draw(float dt)
{
	(void)(dt);

	Sprite::FlushBatch();

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	model.Draw(glm::vec3(0));
	//teapot.Draw(glm::vec3(0));
	MeshBucket::Flush();

	glDisable(GL_DEPTH_TEST);

	Tickable::Draw(dt);
	Sprite::FlushBatch();
}

#endif
