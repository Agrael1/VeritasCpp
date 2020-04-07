#include "SwapChain.h"
#include "Window.h"

SwapChain::SwapChain(SwapChainDesc desc, Window& Wndref)
	:rWnd(Wndref),width(desc.width), height(desc.height)
{
	FrameBuffer.resize((size_t)desc.height * desc.width);
}

void SwapChain::Present()
{
	rWnd.OutputToScreen(FrameBuffer);
}

void SwapChain::ClearFrame(ConsoleColor color)
{
	std::fill(FrameBuffer.begin(), FrameBuffer.end(), color);
}

void SwapChain::PutPixel(uint16_t x, uint16_t y, ConsoleColor color)
{
	FrameBuffer[(size_t)y * width + x] = color;
}
