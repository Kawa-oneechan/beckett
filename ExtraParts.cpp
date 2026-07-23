#include "engine/InputsMap.h"
#include "engine/Shader.h"
#include "ExtraParts.h"
#include "Camera.h"

void Subtitle::AudioEvent(float time, const std::string& text)
{
	this->Text = text;
}

bool FirstPersonController::Tick(float dt)
{
	if (Inputs.KeyDown(Binds::WalkN) || Inputs.KeyDown(Binds::WalkS))
	{
		auto speed = Inputs.KeyDown(Binds::WalkN) ? 0.05f : -0.05f;
		auto pos = MainCamera->GetTarget();
		auto yaw = MainCamera->GetAngles().z;
		pos.x -= glm::sin(glm::radians(yaw)) * speed;
		pos.z -= glm::cos(glm::radians(yaw)) * speed;
		MainCamera->Target(pos);
	}
	if (Inputs.KeyDown(Binds::WalkE) || Inputs.KeyDown(Binds::WalkW))
	{
		auto speed = Inputs.KeyDown(Binds::WalkW) ? 0.025f : -0.025f;
		auto pos = MainCamera->GetTarget();
		auto yaw = MainCamera->GetAngles().z + 90.0f;
		pos.x -= glm::sin(glm::radians(yaw)) * speed;
		pos.z -= glm::cos(glm::radians(yaw)) * speed;
		MainCamera->Target(pos);
	}

	return Tickable::Tick(dt);
}

void Background::Draw(float dt)
{
	(void)(dt);
	Sprite::DrawSprite(Shaders["background"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
}

void BoingBall::Draw(float dt)
{
	(void)(dt);
	Sprite::DrawSprite(Shaders["boingball"], *whiteRect, glm::vec2(0), glm::vec2(width, height), glm::vec4(0, 0, width, height));
}

void Farrah::Draw(float dt)
{
	(void)(dt);
	auto time = commonUniforms.TotalTime * 10.0f;
	auto pos = glm::vec2(BECKETT_SCREENWIDTH, BECKETT_SCREENHEIGHT) * 0.5f;

	Sprite::DrawSprite(stage,
		pos, stage[0], 0.0f,
		glm::vec4(glm::vec3(0.5f + glm::abs(glm::sin(time * 0.1f) * 0.5f)), 1.0f),
		Sprite::SpriteFlags::CenterOrigin
	);
	Sprite::DrawSprite(sprite,
		pos, sprite[(int)time / 2 % sprite.Frames()], 0.0f,
		glm::vec4(1.0f),
		Sprite::SpriteFlags::MidBotOrigin
	);
}

#ifndef BECKETT_NO3DMODELS
//Not a 2D thing but still Crap Storage.
void Teapot::Draw(float dt)
{
	(void)(dt);
	auto time = commonUniforms.TotalTime * 50.0f;

	//Have to flush here so the draw order becomes boingball, teapot, stage.
	Sprite::FlushBatch();
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	model.Draw(glm::vec3(0), time);
	MeshBucket::Flush();

	glDisable(GL_DEPTH_TEST);
}
#endif

void RoryNite::Draw(float dt)
{
	for (int i = trailCt - 1; i >= 0; i--)
	{
		auto p = position + glm::vec2(i * 8, glm::sin((commonUniforms.TotalTime * 8) - i) * 4); //* (trailCt / (float)i));
		Sprite::DrawSprite(sprite, p, sprite[0], 0.0f, glm::vec4(1, 1, 1, 1.0 - (i / (float)trailCt)));
	}
}
