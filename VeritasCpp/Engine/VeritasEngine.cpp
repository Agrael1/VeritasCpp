#include "VeritasEngine.h" 

VeritasEngine::VeritasEngine(uint16_t width, uint16_t height)
	:Window(width,height,L"VTest")
{
	VSWAP_CHAIN_DESC sd;
	sd.BufferCount = 1;
	sd.Height = height;
	sd.Width = width;
	sd.OutputWindow = 0;
	sd.Windowed = TRUE;

	VFCreateDevice(Window.GetWindowHandle(), &pGfx, &pContext);
	VFCreateSwapChain(&sd, pGfx.Get(), &pSwap);
	pSwap->GetRenderTarget(0, &rtv);
	pContext->OMSetRenderTargets(1, &rtv);

	model.emplace(*this);
}

int VeritasEngine::Start()
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

void VeritasEngine::ProcessInput(float dt)
{
	while (const auto e = Window.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_INSERT:
			if (Window.CursorEnabled())
			{
				Window.DisableCursor();
				Window.mouse.EnableRaw();
			}
			else
			{
				Window.EnableCursor();
				Window.mouse.DisableRaw();
			}
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
		}
	}
}
void VeritasEngine::DoFrame(float dt)
{

	pContext->ClearRenderTarget(&rtv, 0xFF00FFFF);

	model->Draw(*this);
	pSwap->Present();
}
