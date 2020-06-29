#pragma once
#include <Engine\Window.h>
#include <Engine\Graphics.h>
#include "PreClipTransform.h"
#include "IndexedTriangleList.h"

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
	Cube cube;
	PC3Transformer pst;

	static constexpr float dTheta = DirectX::XM_2PI;
	float offset_z = 2.0f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
};