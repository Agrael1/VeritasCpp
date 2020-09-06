#include "App.h"

App::App(uint16_t width, uint16_t height) :
	engine(width, height), model(engine)
{
	engine.SetProjection(DirectX::XMMatrixPerspectiveLH(1, float(height) / float(width), 0.5f, 40.0f));
}

int App::Start()
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

void App::ProcessInput(float dt)
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

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed(VK_SHIFT))
		{
			dt *= 2;
		}
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::DoFrame(float dt)
{
	engine.BeginFrame(0.0, 1.0, 0.0);
	engine.SetCamera(cam.GetViewMatrix());

	model.Draw(engine);

	engine.EndFrame();
}
