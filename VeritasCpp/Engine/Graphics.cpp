#include "Graphics.h"

Graphics::Graphics(uint16_t width, uint16_t height, Window& Wndref)
	:Swap({ width, height, 2 }, Wndref)
{
	
}

void Graphics::BeginFrame(ConsoleColor color)
{
	Swap.ClearFrame(color);
}
void Graphics::EndFrame()
{
	Swap.Present();
}
