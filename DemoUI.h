#pragma once
#include <functional>
#include "engine/Tickable.h"

class Button : public Tickable2D
{
public:
	std::string Text;
	glm::vec4 Color{ 1, 1, 1, 1 };
	glm::vec4 BackColor{ 0.35, 0.35, 0.35, 0.5 };
	glm::vec2 Size{ 128, 32 };
	float TextSize{ 100.0f };
	float Angle{ 0.0f };
	int Font{ 0 };
	bool Raw{ false };
	std::function<void()> OnClick{ nullptr };
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	Button(const std::string& text, glm::vec2 position, glm::vec2 size = glm::vec2(-1));

	bool Tick(float dt) override;

	void Draw(float) override;

	glm::vec2 GetMinimalSize() override;

	glm::vec2 GetSize() override;
};

class TestPanel : public Tickable2D
{
public:
	glm::vec4 Color{ 1, 1, 1, 1 };
	glm::vec4 BackColor{ 0.75, 0.75, 0.75, 0.5 };
	glm::vec2 Size;
	float Margin{ 6.0f };
	float Spacing{ 4.0f };
public:
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	explicit TestPanel(glm::vec2 position, glm::vec2 size = glm::vec2(-1));

	void Draw(float dt) override;

	void Reflow();
};
