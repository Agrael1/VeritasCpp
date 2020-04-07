#pragma once
#include <Engine\Window.h>
#include <Engine\Graphics.h>

class VeritasEngine
{
public:
	VeritasEngine(uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth);
public:
	int Start();
private:
	void ProcessInput(float dt);
	void DoFrame(float dt);
private:
	Window Console;
	Graphics Gfx;
};