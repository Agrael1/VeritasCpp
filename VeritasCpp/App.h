#pragma once
#include "Engine\VeritasEngine.h"
#include "Engine\Model.h"
#include "Engine\Camera.h"

class App
{
public:
	App(uint16_t width, uint16_t height);
public:
	int Start();
private:
	void ProcessInput(float dt);
	void DoFrame(float dt);
private:
	VeritasEngine engine;
	Camera cam;
	XModel model;
};