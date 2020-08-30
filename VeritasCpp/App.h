#pragma once
#include "Engine\VeritasEngine.h"
#include "Camera.h"
#include "XModel.h"
#include "Plane.h"

class App
{
public:
	App(uint16_t width, uint16_t height):
		engine(width, height), model(engine)
	{
		engine.SetProjection(DirectX::XMMatrixPerspectiveLH(1, float(height)/float(width), 0.5f, 40.0f));
	}
public:
	int Start()
	{
		float dt = 0.005f;
		while (true)
		{
			const auto a = Window::ProcessMessages();
			if (a)
			{
				return (int)a.value();
			}
			ProcessInput(dt);
			DoFrame(dt);
		}
	}
private:
	void ProcessInput(float dt)
	{
		auto& wnd = engine.Wnd();
		while (const auto e = wnd.kbd.ReadKey())
		{
			if (!e->IsPress())
			{
				continue;
			}

			switch (e->GetCode())
			{
			case VK_INSERT:
				if (wnd.CursorEnabled())
				{
					wnd.DisableCursor();
					wnd.mouse.EnableRaw();
				}
				else
				{
					wnd.EnableCursor();
					wnd.mouse.DisableRaw();
				}
				break;
			case VK_ESCAPE:
				PostQuitMessage(0);
			}
		}
	}
	void DoFrame(float dt)
	{
		engine.BeginFrame(0.0, 1.0, 0.0);
		engine.SetCamera(cam.GetViewMatrix());

		model.Draw(engine);

		engine.EndFrame();
	}
private:
	VeritasEngine engine;
	Camera cam;
	XModel model;
};