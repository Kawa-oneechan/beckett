#ifndef BECKETT_NO3DMODELS
#include "engine/Framebuffer.h"
#include "engine/Shader.h"
#include "3DDemo.h"
#include "DemoUI.h"

extern float fieldOfView;
extern void RecalcProjections();

static bool hdr = false;

ThreeDDemo::ThreeDDemo()
{
	ID = "3D Demo Scene";

	model.SetVisibility("Collision", false);

	if (model.Lights.empty())
	{
		commonUniforms.Lights[0].color = glm::vec4(1.0, 1.0, 1.0, 0.5);
		commonUniforms.Lights[0].pos = glm::vec4(20, 15, 0, 0);
		//commonUniforms.Lights[0].color = glm::vec4(0);
		//commonUniforms.Lights[0].pos = glm::vec4(0);
	}
	else
	{
		commonUniforms.Lights[1].pos = model.Lights[0].Position;
		commonUniforms.Lights[1].color = model.Lights[0].Color;
		commonUniforms.Lights[2].color = glm::vec4(0);
		commonUniforms.Lights[2].pos = glm::vec4(0);
	}

	//Maybe figure out how to get this from a UfbxMisc camera?
	fieldOfView = 22.5f;
	RecalcProjections();

	postFx = new Framebuffer(Shaders["postfx"], width, height);

	if (model.Cameras.empty())
	{
		MainCamera->FirstPerson(false);
		MainCamera->Target(glm::vec3(0));
		MainCamera->Angles(glm::vec3(0, 47, 0));
		MainCamera->Distance(70);
	}
	else
	{
		//MainCamera->FirstPerson(true);
		MainCamera->Target(model.Cameras[0].Position);
		MainCamera->Angles(model.Cameras[0].Direction);
		MainCamera->Distance(0);
	}

	auto hdrButton = std::make_shared<CheckBox>("Turn on HDR", glm::vec2(4));
	hdrButton->OnChange = [&](const auto& btn)
	{
		hdr = btn.Checked;
		//btn.Text = hdr ? "Turn off HDR" : "Turn on HDR";
	};
	hdrButton->Color = glm::vec4(1.0f);
	hdrButton->AbsolutePosition = hdrButton->Position;
	AddChild(hdrButton);

	auto trackBar = std::make_shared<TrackBar>(0, -50, 50, 1, glm::vec2(8, 32), 256.0f);
	trackBar->OnChange = [&](const auto& bar)
	{
		commonUniforms.Lights[1].pos.z = -(float)bar.Value;
	};
	trackBar->AbsolutePosition = trackBar->Position;
	AddChild(trackBar);

	auto trackBar2 = std::make_shared<TrackBar>(0, -50, 50, 1, glm::vec2(8, 64), 256.0f, true);
	trackBar2->OnChange = [&](const auto& bar)
	{
		commonUniforms.Lights[1].pos.x = (float)bar.Value;
	};
	trackBar2->AbsolutePosition = trackBar2->Position;
	AddChild(trackBar2);

	//hdrButton->SetNavTarget(4, trackBar.get());
	//trackBar->SetNavTarget(4, trackBar2.get());
	//trackBar2->SetNavTarget(4, hdrButton.get());
	hdrButton->SetFocus();


	auto scriptedButton = std::make_shared<Button>("Scripted button", glm::vec2(128));
	scriptedButton->AbsolutePosition = scriptedButton->Position;
	scriptedButton->Bind("examples/test.lua");
	AddChild(scriptedButton);
}

/*
bool ThreeDScene::Tick(float dt)
{
	//commonUniforms.Lights[0].pos.x = 20 * glm::cos(commonUniforms.TotalTime * 0.5f);
	//commonUniforms.Lights[0].pos.y = 20 * glm::sin(commonUniforms.TotalTime * 0.5f);
	return Tickable::Tick(dt);
}
*/

ThreeDDemo::~ThreeDDemo()
{
	delete postFx;
}

void ThreeDDemo::Draw(float dt)
{
	(void)(dt);

	Sprite::FlushBatch();

	if (hdr)
	{
		postFx->Use();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	model.Draw(glm::vec3(0));
	//teapot.Draw(glm::vec3(0));
	MeshBucket::Flush();

	glDisable(GL_DEPTH_TEST);

	if (hdr)
	{
		postFx->Drop();
		postFx->Draw(glm::vec2(0), glm::vec2(width, height));
	}

	Tickable::Draw(dt);
	Sprite::FlushBatch();
}

#endif
