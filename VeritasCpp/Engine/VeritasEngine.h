#pragma once
#include <Interfaces.h>
#include <Engine\Window.h>
#include "XModel.h"

using RenderTargetView = VRTV_DESC;

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
	wrl::ComPtr<IVDevice> pGfx;
	wrl::ComPtr<IVContext> pContext;
	wrl::ComPtr<IVSwapChain> pSwap;
	RenderTargetView rtv{ 0 };
};