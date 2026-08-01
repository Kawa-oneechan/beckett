#ifndef BECKETT_NO3DMODELS
#include "engine/Framebuffer.h"
#include "engine/Shader.h"
#include "TrainDemo.h"
#include "DemoUI.h"

extern float fieldOfView;
extern void RecalcProjections();

static bool hdr = false;

TrainDemo::TrainDemo()
{
	ID = "Train Demo Scene";

	/*
	MainCamera->Target(glm::vec3(6, 12, -37));
	MainCamera->Angles(glm::vec3(1, 2, -3));
	MainCamera->Distance(50);
	MainCamera->FirstPerson(false);

	commonUniforms.Lights[0].color = glm::vec4(1.0, 1.0, 1.0, 0.0);
	commonUniforms.Lights[0].pos = glm::vec4(-50, 24, 35, 0);
	commonUniforms.Lights[1].color = glm::vec4(0.75, 0.75, 0.75, 0.04);
	commonUniforms.Lights[1].pos = glm::vec4(16, 22, -16, 0);
	commonUniforms.Lights[2].color = glm::vec4(0.75, 0.75, 0.75, 0.04);
	commonUniforms.Lights[2].pos = glm::vec4(16, 22, -40, 0);
	*/

	MainCamera->FirstPerson(true);
	MainCamera->Target(model.Cameras[0].Position);
	MainCamera->Angles(model.Cameras[0].Direction);
	MainCamera->Distance(50);
	fieldOfView = 45.0f;
	RecalcProjections();

	for (int i = 0; i < 2; i++)
	{
		commonUniforms.Lights[i + 1].pos = model.Lights[i].Position;
		commonUniforms.Lights[i + 1].color = model.Lights[i].Color;
	}

	commonUniforms.Lights[0].color = glm::vec4(1.0, 1.0, 1.0, 0.0);
	commonUniforms.Lights[0].pos = glm::vec4(-50, 24, 35, 0);
	commonUniforms.Toon = false;

	postFx = new Framebuffer(Shaders["postfx"], width, height);

	auto testButton = std::make_shared<Button>("Toggle Bob", glm::vec2(0));
	testButton->OnClick = [&]()
	{
		if (!bob)
			bob = VFS::GetModel("examples/train/bob/bob.fbx");
		else
			bob.reset();
	};
	testButton->AbsolutePosition = testButton->Position;
	AddChild(testButton);

	auto testButton2 = std::make_shared<Button>("Toggle Boob", glm::vec2(0));
	testButton2->OnClick = [&]()
	{
		if (!boob)
			boob = VFS::GetModel("examples/train/bob/bob.fbx");
		else
			boob.reset();
	};
	testButton2->Position = testButton->Position + glm::vec2(0, 24);
	testButton2->AbsolutePosition = testButton2->Position;
	AddChild(testButton2);

	auto hdrButton = std::make_shared<Button>("Turn on HDR", glm::vec2(0));
	hdrButton->OnClick = [&, hdrButton]()
	{
		hdr = !hdr;
		hdrButton->Text = hdr ? "Turn off HDR" : "Turn on HDR";
	};
	hdrButton->AbsolutePosition = testButton->Position + glm::vec2(100, 0);
	AddChild(hdrButton);

}

TrainDemo::~TrainDemo()
{
	delete postFx;
	MainCamera->Shake.y = 0.0f;
}

bool TrainDemo::Tick(float dt)
{
	bumpTimer += dt;
	if (bumpTimer < 2.0f)
		MainCamera->Shake.y = 0.003f;
	else if (bumpTimer < 2.2f)
		MainCamera->Shake.y = 0.010f;
	else
		bumpTimer = 0.0f;

	return Tickable::Tick(dt);
}

void TrainDemo::Draw(float dt)
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

	//commonUniforms.Lights[0].pos.x = 20 * glm::cos(commonUniforms.TotalTime * 1.0f);
	//commonUniforms.Lights[0].pos.y = 20 * glm::sin(commonUniforms.TotalTime * 1.0f);

	if (bob != nullptr)
		bob->Draw(glm::vec3(6, 4, -14), -2.0f);
	if (boob != nullptr)
		boob->Draw(glm::vec3(12, 4, -14), -2.0f);
	model.Draw(glm::vec3(0));

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
