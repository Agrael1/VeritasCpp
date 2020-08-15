#pragma once
#include <cstdint>

enum VBIND_FLAG : uint32_t
{
    VERTEX_BUFFER = 0x1L,
    INDEX_BUFFER = 0x2L,
    CONSTANT_BUFFER = 0x4L,
    SHADER_RESOURCE = 0x8L,
    STREAM_OUTPUT = 0x10L,
    RENDER_TARGET = 0x20L,
    DEPTH_STENCIL = 0x40L,
    UNORDERED_ACCESS = 0x80L,
    DECODER = 0x200L,
    VIDEO_ENCODER = 0x400L
};
enum VFORMAT
{
    FORMAT_NONE = 0,
    FORMAT_UINT16 = 2,
    FORMAT_UINT32 = 4,
	FORMAT_R32G32B32A32_FLOAT,
	FORMAT_R32G32B32_FLOAT,
	FORMAT_R32G32_FLOAT,
	FORMAT_R32_FLOAT
};
enum VPRIMITIVE_TOPOLOGY
{
    UNDEFINED = 0,
    POINTLIST = 1,
    LINELIST = 2,
    LINESTRIP = 3,
    TRIANGLELIST = 4,
    TRIANGLESTRIP = 5,
};
enum VPIXEL_FORMAT
{
	Indexed = 0x00010000 // Indexes into a palette
	, GDI = 0x00020000 // Is a GDI-supported format
	, Alpha = 0x00040000 // Has an alpha component
	, PAlpha = 0x00080000 // Pre-multiplied alpha
	, Extended = 0x00100000 // Extended color 16 bits/channel
	, Canonical = 0x00200000

	, Undefined = 0
	, DontCare = 0

	, Indexed1bpp = (1 | (1 << 8) | Indexed | GDI)
	, Indexed4bpp = (2 | (4 << 8) | Indexed | GDI)
	, Indexed8bpp = (3 | (8 << 8) | Indexed | GDI)
	, GrayScale16bpp = (4 | (16 << 8) | Extended)
	, RGB55516bpp = (5 | (16 << 8) | GDI)
	, RGB56516bpp = (6 | (16 << 8) | GDI)
	, ARGB155516bpp = (7 | (16 << 8) | Alpha | GDI)
	, RGB24bpp = (8 | (24 << 8) | GDI)
	, RGB32bpp = (9 | (32 << 8) | GDI)
	, ARGB32bpp = (10 | (32 << 8) | Alpha | GDI | Canonical)
	, PARGB32bpp = (11 | (32 << 8) | Alpha | PAlpha | GDI)
	, RGB48bpp = (12 | (48 << 8) | Extended)
	, ARGB64bpp = (13 | (64 << 8) | Alpha | Canonical | Extended)
	, PARGB64bpp = (14 | (64 << 8) | Alpha | PAlpha | Extended)
	, CMYK32bpp = (15 | (32 << 8))
	, Max = 16
};

typedef struct VBUFFER_DESC
{
	uint32_t        ByteWidth;
	//enum usage Usage; //will be in next release
	uint32_t        BindFlags;
	//UINT        CPUAccessFlags;
	uint32_t        StructureByteStride;
} VBUFFER_DESC;
typedef struct VINPUT_ELEMENT
{
    VFORMAT Format;
    uint32_t InputSlot;
    uint32_t AlignedByteOffset;
}VINPUT_ELEMENT;
typedef struct VTEXTURE_DESC
{
    uint32_t Width;
    uint32_t Height;
	VPIXEL_FORMAT PixelFormat;
    uint32_t BindFlags;
}VTEXTURE_DESC;

constexpr auto MaxRenderTargets = 2u;
constexpr auto MaxViewPorts = 2u;

typedef struct VVIEWPORT_DESC
{
	uint32_t Width;
	uint32_t Height;
	uint32_t TopLeftX;
	uint32_t TopLeftY;
}VVIEWPORT_DESC;
typedef struct VRTV_DESC
{
	uint32_t Width;
	uint32_t Height;
	uint32_t Stride;
	VPIXEL_FORMAT PixelFormat;
	void* Scan0;
	size_t Reserved;
}VRTV_DESC, VDSV_DESC;

typedef struct VSWAP_CHAIN_DESC
{
	uint32_t Width;
	uint32_t Height;
	uint32_t BufferCount;
	void* OutputWindow;
	int32_t Windowed;
}VSWAP_CHAIN_DESC;