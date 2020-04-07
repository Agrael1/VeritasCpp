#pragma once
#include "SwapChain.h"

class Graphics
{
public:
	Graphics(uint16_t width, uint16_t height, class Window& Wndref);
public:
	void BeginFrame(ConsoleColor color);
	void EndFrame();
private:
	SwapChain Swap;
};