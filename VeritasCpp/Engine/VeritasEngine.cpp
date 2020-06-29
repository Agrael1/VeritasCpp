#include "VeritasEngine.h" 
#include "Color.h"

VeritasEngine::VeritasEngine(uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth)
	:Gfx(width, height, Console),
	cube(1.0f), pst(width, height)
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
		if (Console.kbd.KeyIsPressed('Q'))
		{
			theta_z = wrap_angle(theta_z + dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('W'))
		{
			theta_x = wrap_angle(theta_x + dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('E'))
		{
			theta_z = wrap_angle(theta_z - dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('A'))
		{
			theta_y = wrap_angle(theta_y + dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('S'))
		{
			theta_x = wrap_angle(theta_x - dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('D'))
		{
			theta_y = wrap_angle(theta_y - dTheta * dt);
		}
		if (Console.kbd.KeyIsPressed('R'))
		{
			offset_z += 2.0f * dt;
		}
		if (Console.kbd.KeyIsPressed('F'))
		{
			offset_z -= 2.0f * dt;
		}
}
void VeritasEngine::DoFrame(float dt)
{
	using namespace DirectX;
	Gfx.BeginFrame({ PIXEL_TYPE::PIXEL_NONE, DEF_COLOUR::DBG_BLACK });

	Gfx.DrawTriangle({ 0,0 }, { 150, 250 }, { 5,150 }, { PIXEL_TYPE::PIXEL_NONE, DEF_COLOUR::DBG_BLUE });

	auto triangles = cube.GetTriangles();
	auto colors = cube.GetColors();
	const auto rot = 
		RotationX(theta_x) *
		RotationY(theta_y) *
		RotationZ(theta_z);

	for (auto& v : triangles.vertices)
	{
		v = XMVector3TransformCoord(v,rot);
		v = v + vec3A{ 0.0f, 0.0f, offset_z };
		pst.Transform(v);
	}
	uint16_t j = 0;
	for (auto i = triangles.indices.begin(),
		end = triangles.indices.end();
		i != end; std::advance(i, 3), j++)
	{
		Gfx.DrawTriangle(triangles.vertices[*i], triangles.vertices[*std::next(i)], triangles.vertices[*std::next(i, 2)], {DEF_COLOUR::DBG_WHITE});
	}

	Gfx.EndFrame();
}
