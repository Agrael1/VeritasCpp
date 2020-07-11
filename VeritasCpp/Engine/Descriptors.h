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
    FORMAT_UINT32 = 4
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
typedef struct VBUFFER_DESC
{
	uint32_t        ByteWidth;
	//enum usage Usage; //will be in next release
	uint32_t        BindFlags;
	//UINT        CPUAccessFlags;
	uint32_t        StructureByteStride;
} VBUFFER_DESC;