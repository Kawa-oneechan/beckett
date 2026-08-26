#include "engine/SpriteRenderer.h"
#include "engine/InputsMap.h"
#include "engine/NineSlicer.h"
#include "engine/Font.h"
#include "engine/Audio.h"
#include "DemoUI.h"

extern Texture* whiteRect;

static std::shared_ptr<Sound> clickSound;

static void FrameDrawer(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int flags)
{
	static Texture panels[] = {
		Texture{ "ui/panels/panel3.png" },
		Texture{ "ui/panels/panel4.png" },
		Texture{ "ui/panels/panel5.png" },
		Texture{ "ui/panels/panel6.png" },
	};

	NineSlicer::Draw(panels[flags], (glm::vec2&)pos, (glm::vec2&)size, 1.0f, color);

	/*
	Sprite::DrawSprite(*whiteRect, pos, size - glm::vec2(1), glm::vec4(0), 0.0f, color);

	auto diff = glm::vec4(0.25, 0.25, 0.25, 0.0);
	auto hi = color + diff;
	auto lo = color - diff;

	Sprite::DrawLine(pos, pos + glm::vec2(size.x - 1, 0), hi);
	Sprite::DrawLine(pos + glm::vec2(0, 1), pos + glm::vec2(0, size.y - 1), hi);
	Sprite::DrawLine(pos + glm::vec2(0, size.y - 1), pos + glm::vec2(size.x - 1, size.y - 1), lo);
	Sprite::DrawLine(pos + glm::vec2(size.x, 1), pos + glm::vec2(size.x, size.y - 1), lo);
	*/
}

bool UIControl::IsHovered()
{
	return
		Inputs.MousePosition.x > AbsolutePosition.x &&
		Inputs.MousePosition.y > AbsolutePosition.y &&
		Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
		Inputs.MousePosition.y < AbsolutePosition.y + Size.y;
}

bool UIControl::IsClicked()
{
	return
		Inputs.LastClickLeft.x > AbsolutePosition.x &&
		Inputs.LastClickLeft.y > AbsolutePosition.y &&
		Inputs.LastClickLeft.x < AbsolutePosition.x + Size.x &&
		Inputs.LastClickLeft.y < AbsolutePosition.y + Size.y;
}

Button::Button(const std::string& text, glm::vec2 position, glm::vec2 size) : Text(text)
{
	parent = nullptr;
	Position = position;
	Size = size;

	if (!clickSound)
		clickSound = std::make_shared<Sound>("ui/click5.ogg");

	auto minSize = Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
	if (Size.x == -1)
		Size.x = minSize.x;
	if (Size.y == -1)
		Size.y = minSize.y;
}

bool Button::Tick(float dt)
{
	if (!OnClick)
		return true;
	if (IsClicked())
	{
		Inputs.LastClickLeft = glm::vec2(-1000);
		clickSound->Play(true);
		OnClick(*this);
		return false;
	}
	return true;
}

void Button::Draw(float)
{
	auto frame = 0;
	if (IsHovered())
	{
		frame = 2;
		if (Inputs.MouseHoldLeft)
			frame = 1;
	}
		
	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, BackColor, frame);

	auto size = Sprite::MeasureText(Font, Text, TextSize, Raw);
	auto center = (Size * 0.5f) - (size * 0.5f);
	Sprite::DrawText(Font, Text, AbsolutePosition + center, Color, TextSize, Angle, Raw);
}

glm::vec2 Button::GetMinimalSize()
{
	return Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
}

FlowPanelV::FlowPanelV(glm::vec2 position, glm::vec2 size)
{
	parent = nullptr;
	Position = position;
	Size = size;

	if (Size.x == -1) Size.x = 32;
	if (Size.y == -1) Size.y = 32;

	Size += glm::vec2(8);
}

void FlowPanelV::Draw(float dt)
{
	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, BackColor, 3);

	Tickable2D::Draw(dt);
}

void FlowPanelV::Reflow()
{
	UpdatePosition();
	Tick(0.0f);
	auto pos = glm::vec2(Margin);
	for (int i = 0; i < ChildTickables.size(); i++)
	{
		if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
		{
			t2D->Position = pos;
			t2D->UpdatePosition();
			pos.y += t2D->GetSize().y + Spacing;
		}
	}
	Size = GetMinimalSize() + glm::vec2(Margin);
}

void FlowPanelH::Reflow()
{
	UpdatePosition();
	Tick(0.0f);
	auto pos = glm::vec2(Margin);
	for (int i = 0; i < ChildTickables.size(); i++)
	{
		if (auto t2D = std::dynamic_pointer_cast<Tickable2D>(ChildTickables[i]))
		{
			t2D->Position = pos;
			t2D->UpdatePosition();
			pos.x += t2D->GetSize().x + Spacing;
		}
	}
	Size = GetMinimalSize() + glm::vec2(Margin);
}

TrackBar::TrackBar(int value, int min, int max, int step, glm::vec2 position, float length, bool vertical) : Value(value), Min(min), Max(max), Step(step), vertical(vertical)
{
	parent = nullptr;
	Position = position;
	Size = vertical ? glm::vec2(Thickness, length) : glm::vec2(length, Thickness);
}

bool TrackBar::Tick(float dt)
{
	if (IsHovered())
	{
		if (!Inputs.MouseHoldLeft)
			return true;
		auto oldVal = Value;

		auto mp = !vertical ? Inputs.MousePosition.x : Inputs.MousePosition.y;
		auto ap = !vertical ? AbsolutePosition.x : AbsolutePosition.y;
		auto sz = !vertical ? Size.x : Size.y;

		auto x = glm::clamp(mp, ap, ap + sz);
		auto v = Min + ((x - ap) * (Max - Min)) / sz;
		Value = glm::clamp((int)(round(v / Step) * Step), Min, Max);

		if (Value != oldVal)
			clickSound->Play(true);
		if (OnChange)
			OnChange(*this);
		return false;
	}
	return true;
}

void TrackBar::Draw(float)
{
	auto frame = 0;
	if (IsHovered())
	{
		frame = 2;
		if (Inputs.MouseHoldLeft)
			frame = 1;
	}

	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, TrackColor, 3);

	auto range = Max - Min;
	auto ccur = glm::clamp(Value, Min, Max) - Min;
	if (!vertical)
	{
		auto thumbPos = ((ccur * (Size.x - (Size.y * 0.5f))) / range);
		OnDraw(AbsolutePosition + glm::vec2(thumbPos, 0), glm::vec2(Size.y * 0.5f, Size.y), Color, frame);
	}
	else
	{
		auto thumbPos = ((ccur * (Size.y - (Size.x * 0.5f))) / range);
		OnDraw(AbsolutePosition + glm::vec2(0, thumbPos), glm::vec2(Size.x, Size.x * 0.5f), Color, frame);
	}
}
