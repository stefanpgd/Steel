#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class Project;

class Renderer
{
public:
	Renderer(const std::wstring& applicationName, unsigned int windowWidth, unsigned int windowHeight);
	
	void Render(Project* project);
	void Resize();

private:
	void InitializeImGui();
};