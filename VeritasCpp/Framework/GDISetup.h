#pragma once
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

#ifndef FULL_WINOPT
//#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
//#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

//#define NOMINMAX
#define STRICT

#include <Windows.h>
#include <gdiplus.h>


enum PixelFormat : unsigned
{
	Indexed 		= 0x00010000 // Indexes into a palette
	,GDI     		= 0x00020000 // Is a GDI-supported format
	,Alpha   		= 0x00040000 // Has an alpha component
	,PAlpha			= 0x00080000 // Pre-multiplied alpha
	,Extended		= 0x00100000 // Extended color 16 bits/channel
	,Canonical		= 0x00200000 
	
	,Undefined      = 0
	,DontCare       = 0
	
	,Indexed1bpp    =(1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
	,Indexed4bpp    =(2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
	,Indexed8bpp    =(3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
	,GrayScale16bpp =(4 | (16 << 8) | PixelFormatExtended)
	,RGB55516bpp	=(5 | (16 << 8) | PixelFormatGDI)
	,RGB56516bpp	=(6 | (16 << 8) | PixelFormatGDI)
	,ARGB155516bpp  =(7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
	,RGB24bpp		=(8 | (24 << 8) | PixelFormatGDI)
	,RGB32bpp		=(9 | (32 << 8) | PixelFormatGDI)
	,ARGB32bpp		=(10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
	,PARGB32bpp		=(11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
	,RGB48bpp		=(12 | (48 << 8) | PixelFormatExtended)
	,ARGB64bpp		=(13 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
	,PARGB64bpp		=(14 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
	,CMYK32bpp		=(15 | (32 << 8))
	,Max            =16
};