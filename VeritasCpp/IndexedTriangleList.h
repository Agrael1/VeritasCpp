#pragma once
#include <vector>
#include <array>
#include "XMHelpers.h"

struct IndexedTriangleList
{
	std::vector<vec3A> vertices;
	std::vector<unsigned short> indices;
};



class Cube
{
public:
	Cube(float size)
	{
		const float side = size / 2.0f;
		vertices.emplace_back(-side, -side, -side);
		vertices.emplace_back(side, -side, -side);
		vertices.emplace_back(-side, side, -side);
		vertices.emplace_back(side, side, -side);
		vertices.emplace_back(-side, -side, side);
		vertices.emplace_back(side, -side, side);
		vertices.emplace_back(-side, side, side);
		vertices.emplace_back(side, side, side);

		colors[0] = { DEF_COLOUR::DBG_BLUE };
		colors[1] = { DEF_COLOUR::DBG_CYAN };
		colors[2] = { DEF_COLOUR::DBG_DARK_BLUE };
		colors[3] = { DEF_COLOUR::DBG_DARK_CYAN };
		colors[4] = { DEF_COLOUR::DBG_DARK_GREEN };
		colors[5] = { DEF_COLOUR::DBG_DARK_GREY };
		colors[6] = { DEF_COLOUR::DBG_DARK_MAGENTA };
		colors[7] = { DEF_COLOUR::DBG_DARK_RED };
		colors[8] = { DEF_COLOUR::DBG_DARK_YELLOW };
		colors[9] = { DEF_COLOUR::DBG_GREEN };
		colors[10] = { DEF_COLOUR::DBG_MAGENTA };
		colors[11] = { DEF_COLOUR::DBG_YELLOW };
	}
	IndexedTriangleList GetTriangles() const
	{
		return{
			vertices,{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4 }
		};
	}
	const std::array<ConsoleColor, 12>& GetColors()const noexcept
	{
		return colors;
	}
private:
	std::vector<vec3A> vertices;
	std::array<ConsoleColor, 12> colors;
};