#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

#include <string>

class Renderer;
class Editor;

// When using the template, rename 'Application' to whatever the current project is called 
class Engine
{
public:
	Engine();

	void Run();

private:
	void RegisterWindowClass();

	void Start();
	void Update(float deltaTime);
	void Render();

	static LRESULT CALLBACK WindowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	// TODO: Add reference to active project

	std::wstring frameworkName = L"Steel";
	bool runEngine = true;

	unsigned int windowWidth = 1080;
	unsigned int windowHeight = 720;

	// Systems //
	Renderer* renderer;
	Editor* editor;
};