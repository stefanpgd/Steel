#pragma once

#include <array>

class Editor
{
public:
	Editor();

	void Update(float deltaTime);

private:
	void ImGuiStyleSettings();

private:
	float deltaTime;
	std::array<float, 300> deltaTimeLog;
	unsigned int frameCount = 0;

	struct ImFont* baseFont;
	struct ImFont* boldFont;
};