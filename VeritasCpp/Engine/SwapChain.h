#pragma once
#include <Framework\WinSetup.h>
#include <cstdint>
#include <vector>
#include "Color.h"

struct SwapChainDesc
{
	uint16_t width;
	uint16_t height;
	uint16_t nBuffers;
};

class SwapChain
{
public:
	SwapChain(SwapChainDesc desc, class Window& Wndref);
public:
	void Present();
	void ClearFrame(ConsoleColor color);
	void PutPixel(uint16_t x, uint16_t y, ConsoleColor color);
private:
	std::vector<CHAR_INFO> FrameBuffer;
	uint16_t width;
	uint16_t height;
	Window& rWnd;
};