#pragma once
#include <functional>
#include "engine/Tickable.h"
#include "Scriptable.h"

class UIControl : public Tickable2D
{
public:
	glm::vec2 Size{ 128, 32 };
	class UIControl* navTarget[5] { nullptr };
	bool TabStop{ false };

	~UIControl() override;
	bool Scancode(unsigned int sc) override;

	glm::vec2 GetSize() override { return Size; }

	bool IsHovered() const;
	bool IsClicked() const;
	bool IsFocused() const;
	void SetFocus();

	class UIControl* GetNavTarget(int direction);
	void SetNavTarget(int direction, class UIControl* target);
};

class Button : public UIControl, public Scriptable
{
public:
	std::string Text;
	glm::vec4 Color{ 0.0, 0.0, 0.0, 1.0 };
	glm::vec4 BackColor{ 1.0, 1.0, 1.0, 1.0 };
	float TextSize{ 100.0f };
	float Angle{ 0.0f };
	int Font{ 0 };
	bool Raw{ false };
	std::function<void(Button&)> OnClick{ nullptr };
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	Button(const std::string& text, glm::vec2 position, glm::vec2 size = glm::vec2(-1));

	bool Tick(float dt) override;
	void Draw(float) override;
	bool Scancode(unsigned int sc) override;
	glm::vec2 GetMinimalSize() override;
};

class Panel : public UIControl
{
public:
	glm::vec4 Color{ 0.0, 0.0, 0.0, 1.0 };
	glm::vec4 BackColor{ 0.65, 0.65, 0.65, 1.0 };
	float Margin{ 6.0f };
	float Spacing{ 4.0f };
public:
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	explicit Panel(glm::vec2 position, glm::vec2 size = glm::vec2(-1));

	void Draw(float dt) override;

	virtual void Reflow() {};
};

class FlowPanelV : public Panel
{
public:
	explicit FlowPanelV(glm::vec2 position, glm::vec2 size = glm::vec2(-1)) : Panel(position, size) {};
	void Reflow() override;
};

class FlowPanelH : public Panel
{
public:
	explicit FlowPanelH(glm::vec2 position, glm::vec2 size = glm::vec2(-1)) : Panel(position, size) {};
	void Reflow() override;
};

class TrackBar : public UIControl
{
private:
	bool vertical{ false };
public:
	int Value, Min, Max, Step;
	const int Thickness{ 24 };
	glm::vec4 Color{ 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 TrackColor { 0.5, 0.5, 0.5, 1.0 };
	std::function<void(TrackBar&)> OnChange{ nullptr };
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	TrackBar(int value, int min, int max, int step, glm::vec2 position, float length =  256.0f, bool vertical = false);

	bool Tick(float dt) override;
	void Draw(float) override;
	bool Scancode(unsigned int sc) override;
};

class CheckBox : public UIControl
{
public:
	std::string Text;
	glm::vec4 Color{ 0.0, 0.0, 0.0, 1.0 };
	float TextSize{ 100.0f };
	float Angle{ 0.0f };
	int Font{ 0 };
	bool Raw{ false };
	bool Checked{ false };
	std::function<void(CheckBox&)> OnChange{ nullptr };
	std::function<void(const glm::vec2&, const glm::vec2&, const glm::vec4&, int)> OnDraw{ nullptr };

	CheckBox(const std::string& text, glm::vec2 position);

	bool Tick(float dt) override;
	void Draw(float) override;
	bool Scancode(unsigned int sc) override;
};

extern void SetupNavTargets(Tickable* root);
