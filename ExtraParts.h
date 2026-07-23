#pragma once
#include "engine/Tickable.h"
#include "engine/Model.h"
#include "engine/Audio.h"

class FirstPersonController : public Tickable
{
	bool Tick(float dt) override;
};

class Subtitle : public TextLabel, public AudioEventListener
{
public:
	void AudioEvent(float time, const std::string& text) override;
	Subtitle(const std::string& text, glm::vec2 position) : TextLabel(text, position) {}
};

class Background : public Tickable
{
public:
	void Draw(float dt) override;
};

class BoingBall : public Tickable
{
public:
	void Draw(float dt) override;
};

class Farrah : public Tickable
{
private:
	Texture sprite{ "examples/2dscene/farrah.png" };
	Texture stage{ "examples/2dscene/stage.png" };

public:
	void Draw(float dt) override;
};

//TODO: Make this a Tickable2D that spawns SimpleSprites, more like
//how Deltarune does the effect.
class RoryNite : public Tickable
{
private:
	Texture sprite{ "examples/rorynite.png" };
	static constexpr int trailCt = 8;
public:
	glm::vec2 position = glm::vec2(BECKETT_SCREENWIDTH / 2, BECKETT_SCREENHEIGHT / 2);

	void Draw(float dt) override;
};

#ifndef BECKETT_NO3DMODELS
class Teapot : public Tickable
{
private:
	Model model{ "examples/teapot.fbx" };

public:
	void Draw(float dt) override;
};
#endif
