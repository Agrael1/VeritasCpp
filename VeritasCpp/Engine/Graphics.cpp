#include "Graphics.h"

Graphics::Graphics(uint16_t width, uint16_t height, Window& Wndref)
	:Swap({ width, height, 2 }, Wndref)
{
	
}

void Graphics::BeginFrame(ConsoleColor color)
{
	Swap.ClearFrame(color);
}
void Graphics::DrawTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color)
{
    using namespace DirectX;
	const auto* pv0 = &v0;
	const auto* pv1 = &v1;
	const auto* pv2 = &v2;

    if (pv1->y < pv0->y) std::swap(pv0, pv1);
    if (pv2->y < pv1->y) std::swap(pv1, pv2);
    if (pv1->y < pv0->y) std::swap(pv0, pv1);

    if (pv0->y == pv1->y) // natural flat top
    {
        // sorting top vertices by x
        if (pv1->x < pv0->x) std::swap(pv0, pv1);

        DrawFlatTopTriangle(*pv0, *pv1, *pv2, color);
    }
    else if (pv1->y == pv2->y) // natural flat bottom
    {
        // sorting bottom vertices by x
        if (pv2->x < pv1->x) std::swap(pv1, pv2);

        DrawFlatBottomTriangle(*pv0, *pv1, *pv2, color);
    }
    else // general triangle
    {
        // find splitting vertex interpolant
        const float alphaSplit =
            (pv1->y - pv0->y) /
            (pv2->y - pv0->y);
        const vec2A vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

        if (pv1->x < vi.x) // major right
        {
            DrawFlatBottomTriangle(*pv0, *pv1, vi, color);
            DrawFlatTopTriangle(*pv1, vi, *pv2, color);
        }
        else // major left
        {
            DrawFlatBottomTriangle(*pv0, vi, *pv1, color);
            DrawFlatTopTriangle(vi, *pv1, *pv2, color);
        }
    }
}
void Graphics::EndFrame()
{
	Swap.Present();
}

void Graphics::DrawFlatTopTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color)
{
    //calculate the slope <!notice the order x/y!>
    float m0 = (v2.x - v0.x) / (v2.y - v0.y);
    float m1 = (v2.x - v1.x) / (v2.y - v1.y);

    //calculate start and end scanlines
    const int yStart = (int)ceil(v0.y - 0.5f);
    const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline after the last line
    
    for (int y = yStart; y < yEnd; y++)
    {
        const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
        const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

        const int xStart = int(ceil(px0 - 0.5f));
        const int xEnd = int(ceil(px1 - 0.5f)); //the pixel after the last drawn

        for (int x = xStart; x < xEnd; x++)
        {
            Swap.PutPixel(x, y, color);
        }
    }
}
void Graphics::DrawFlatBottomTriangle(const vec2A& v0, const vec2A& v1, const vec2A& v2, ConsoleColor color)
{
    //calculate the slope <!notice the order x/y!>
    float m0 = (v1.x - v0.x) / (v1.y - v0.y);
    float m1 = (v2.x - v0.x) / (v2.y - v0.y);

    //calculate start and end scanlines
    const int yStart = (int)ceil(v0.y - 0.5f);
    const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline after the last line

    for (int y = yStart; y < yEnd; y++)
    {
        const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
        const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

        const int xStart = int(ceil(px0 - 0.5f));
        const int xEnd = int(ceil(px1 - 0.5f)); //the pixel after the last drawn

        for (int x = xStart; x < xEnd; x++)
        {
            Swap.PutPixel(x, y, color);
        }
    }
}
