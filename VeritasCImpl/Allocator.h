#pragma once
#include "RuntimeClass.h"
#include <VA_OPT.h>
#include <malloc.h>
#include <assert.h>

typedef struct _Allocator
{
	void (*vdtor)(void* self);
	unsigned char __class[];
}Allocator;

#define MakeAndInitialize(OUT_ppInterface, What,...)					\
do																		\
{																		\
	Allocator* p = calloc(1, sizeof(struct What) + sizeof(Allocator));	\
																		\
	assert(p);															\
	p->vdtor = (void(*)(void*))&__rcat3(What, _, dtor);					\
	struct What* pClass = (struct What*)((unsigned char*)p + sizeof(Allocator));						\
																		\
	pClass->__internal_refcount = 1;									\
	pClass->vfptr.method = __rcat2(__,__rcat3(What,_,vftbl));			\
	__rcat3(What, _, ctor)(pClass IFN(__VA_ARGS__)(,__VA_ARGS__) );		\
																		\
	*OUT_ppInterface = (void*)pClass;									\
}while(0)

HRESULT __stdcall QueryInterface(IUnknown* This, void* riid, void** ppvObject);
uint32_t __stdcall AddRef(IUnknown* This);
uint32_t __stdcall Release(IUnknown* This);

#define __COM_IUnknown 					  \
{										  \
	.QueryInterface = QueryInterface,	  \
	.AddRef = AddRef,					  \
	.Release = Release					  \
}										  