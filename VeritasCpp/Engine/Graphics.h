#pragma once
#include "SwapChain.h"
#include "XMHelpers.h"



class Graphics
{
public:
	Graphics(uint16_t width, uint16_t height, class Window& Wndref);
public:
	void BeginFrame(ConsoleColor color);
	void DrawTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color);
	void EndFrame();
private:
	void DrawFlatTopTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color);
	void DrawFlatBottomTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color);
private:
	SwapChain Swap;
};