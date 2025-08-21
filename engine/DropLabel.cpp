#include <glad/glad.h>
#include "DropLabel.h"
#include "SpriteRenderer.h"

extern int width, height;

void DropLabel::update()
{
	if (canvas.ID != 0)
		glDeleteTextures(1, &canvas.ID);

	Sprite::FlushBatch();

	size = Sprite::MeasureText(2, text, 100.0);
	size += glm::vec2(16);

	int width = (int)size.x, height = (int)size.y;
	unsigned int finalTID;

	unsigned int tempFBO, tempTID, blurFBO, blurTID, finalFBO;

	glGenTextures(1, &finalTID);
	canvas.ID = finalTID;
	canvas.width = width;
	canvas.height = height;
	canvas.channels = 4;
	canvas.SetFilter(GL_NEAREST);

	//Step 1 - Draw text to TEMP
	{
		glGenTextures(1, &tempTID);

		glGenFramebuffers(1, &tempFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, tempFBO);

		glBindTexture(GL_TEXTURE_2D, tempTID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTID, 0);

		glViewport(0, 0, width, height);

		glColorMask(true, true, true, true);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Sprite::DrawText(2, text, glm::vec2(8), glm::vec4(1), 100.0f);
		Sprite::FlushBatch();
	}

	::Texture tempTexture(tempTID, width, height, 4);

	const int blurDiv = 2;

	//Step 2 - Draw TEMP to BLUR
	{

		glGenTextures(1, &blurTID);

		glGenFramebuffers(1, &blurFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

		glBindTexture(GL_TEXTURE_2D, blurTID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / blurDiv, height / blurDiv, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTID, 0);

		glViewport(0, 0, width / blurDiv, height / blurDiv);

		glColorMask(true, true, true, true);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Sprite::DrawSprite(tempTexture, glm::vec2(0), glm::vec2(width / blurDiv, height / blurDiv), glm::vec4(0, 0, width, height), 0.0f, glm::vec4(0, 0, 0, 1));
		Sprite::FlushBatch();
	}

	::Texture blurTexture(blurTID, width / 2, height / 2, 4);

	//Step 3 - Draw BLUR and TEMP to FINAL
	{

		glGenFramebuffers(1, &finalFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, finalFBO);

		glBindTexture(GL_TEXTURE_2D, finalTID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalTID, 0);

		glViewport(0, 0, width, height);

		glColorMask(true, true, true, true);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const float disp = 2.0f;

		for (int i = -disp; i < disp; i++)
			for (int j = -disp; j < disp; j++)
				Sprite::DrawSprite(blurTexture, glm::vec2(i, j), glm::vec2(width, height));
		Sprite::DrawSprite(tempTexture, glm::vec2(0), glm::vec2(width, height));
		Sprite::FlushBatch();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ::width, ::height);

	glDeleteTextures(1, &tempTID);
	glDeleteTextures(1, &blurTID);
	glDeleteFramebuffers(1, &tempFBO);
	glDeleteFramebuffers(1, &blurFBO);
	glDeleteFramebuffers(1, &finalFBO);
}

DropLabel::DropLabel(const std::string& text) : text(text)
{
	update();
}

void DropLabel::SetText(const std::string& text)
{
	if (this->text != text)
	{
		this->text = text;
		update();
	}
}
