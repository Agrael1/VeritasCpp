#include "VeritasEngine.h" 
#include "Color.h"

VeritasEngine::VeritasEngine(uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth)
	:Gfx(width, height, Console)
{
	Console.CreateConsole(width, height, fontw, fonth);
}

int VeritasEngine::Start()
{
	float dt = 0.005f;
	while (true)
	{
		if (!Console.InFocus())
			Console.CatchFocus();

		//dt += 0.005f;
		const auto a = Window::ProcessMessages();
		if (a)
		{
			return (int)a.value();
		}
		ProcessInput(dt);
		DoFrame(dt);
	}
}

void VeritasEngine::ProcessInput(float dt)
{
	while (const auto e = Console.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_INSERT:
			if (Console.CursorEnabled())
			{
				Console.DisableCursor();
				Console.mouse.EnableRaw();
			}
			else
			{
				Console.EnableCursor();
				Console.mouse.DisableRaw();
			}
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
		}
	}
}
void VeritasEngine::DoFrame(float dt)
{
	Gfx.BeginFrame({ PIXEL_TYPE::PIXEL_NONE, DEF_COLOUR::DBG_BLUE });



	Gfx.EndFrame();
}
