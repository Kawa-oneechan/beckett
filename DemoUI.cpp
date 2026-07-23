#include "engine/SpriteRenderer.h"
#include "engine/InputsMap.h"
#include "DemoUI.h"

extern Texture* whiteRect;

static void FrameDrawer(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, int flags)
{
	Sprite::DrawSprite(*whiteRect, pos, size - glm::vec2(1), glm::vec4(0), 0.0f, color);

	auto diff = glm::vec4(0.25, 0.25, 0.25, 0.0);
	auto hi = color + diff;
	auto lo = color - diff;

	Sprite::DrawLine(pos, pos + glm::vec2(size.x - 1, 0), hi);
	Sprite::DrawLine(pos + glm::vec2(0, 1), pos + glm::vec2(0, size.y - 1), hi);
	Sprite::DrawLine(pos + glm::vec2(0, size.y - 1), pos + glm::vec2(size.x - 1, size.y - 1), lo);
	Sprite::DrawLine(pos + glm::vec2(size.x, 1), pos + glm::vec2(size.x, size.y - 1), lo);
}

Button::Button(const std::string& text, glm::vec2 position, glm::vec2 size) : Text(text), Size(size)
{
	parent = nullptr;
	Position = position;

	auto minSize = Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
	if (Size.x == -1)
		Size.x = minSize.x;
	if (Size.y == -1)
		Size.y = minSize.y;
}

bool Button::Tick(float dt)
{
	if (!Inputs.MouseLeft)
		return true;
	if (!OnClick)
		return true;
	if (Inputs.MousePosition.x > AbsolutePosition.x &&
		Inputs.MousePosition.y > AbsolutePosition.y &&
		Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
		Inputs.MousePosition.y < AbsolutePosition.y + Size.y)
	{
		OnClick();
		return false;
	}
	return true;
}

void Button::Draw(float)
{
	auto color = BackColor;
	if (Inputs.MousePosition.x > AbsolutePosition.x &&
		Inputs.MousePosition.y > AbsolutePosition.y &&
		Inputs.MousePosition.x < AbsolutePosition.x + Size.x &&
		Inputs.MousePosition.y < AbsolutePosition.y + Size.y)
		color.a = 1.0f;

	//color = glm::vec4(HSLtoRGB((glm::cos(commonUniforms.TotalTime * 0.75f) * 0.5f) + 0.5f, 0.75f, 0.5f), 1.0f);

	//Sprite::DrawSprite(*whiteRect, AbsolutePosition, Size, glm::vec4(0), 0.0f, color);
	//Sprite::DrawRect(glm::vec4(AbsolutePosition, AbsolutePosition + Size), Color);
	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, color, 0);

	auto size = Sprite::MeasureText(Font, Text, TextSize, Raw);
	auto center = (Size * 0.5f) - (size * 0.5f);
	Sprite::DrawText(Font, Text, AbsolutePosition + center, Color, TextSize, Angle, Raw);
}

glm::vec2 Button::GetMinimalSize()
{
	return Sprite::MeasureText(Font, Text, TextSize, Raw) + glm::vec2(16, 8);
}

glm::vec2 Button::GetSize()
{
	return Size;
}

TestPanel::TestPanel(glm::vec2 position, glm::vec2 size) : Size(size)
{
	parent = nullptr;
	Position = position;

	//auto minSize = GetMinimalSize();
	if (Size.x == -1)
		Size.x = 32; //minSize.x;
	if (Size.y == -1)
		Size.y = 32; //minSize.y;

	Size += glm::vec2(8);
}

void TestPanel::Draw(float dt)
{
	//Sprite::DrawSprite(*whiteRect, AbsolutePosition, Size, glm::vec4(0), 0.0f, BackColor);
	//Sprite::DrawRect(glm::vec4(AbsolutePosition, AbsolutePosition + Size), Color);
	if (!OnDraw)
		OnDraw = FrameDrawer;
	OnDraw(AbsolutePosition, Size, BackColor, 0);

	Tickable2D::Draw(dt);
}

void TestPanel::Reflow()
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
