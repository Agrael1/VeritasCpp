#pragma once
#include <Engine\Graphics.h>
#include <Engine\SwapChain.h>
#include <Engine\Context.h>
#include <Engine\Window.h>



class VeritasEngine
{
	friend class Bindable;
public:
	VeritasEngine(uint16_t width, uint16_t height);
public:
	int Start();
private:
	void ProcessInput(float dt);
	void DoFrame(float dt);
private:
	Window Window;
	VGraphicsDevice gfx;
	VSwapChain swap;
	VContext context;
	RenderTargetView rtv{ 0 };
};