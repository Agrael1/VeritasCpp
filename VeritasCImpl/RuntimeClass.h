#pragma once
#include "CInterfaces.h"

#define __xcat2(x,y)x##y
#define __rcat2(x,y) __xcat2(x,y)
#define __xcat3(x,y,z)x##y##z
#define __rcat3(x,y,z) __xcat3(x,y,z)

#define __rctor(x) __rcat3(x,_,ctor)
#define RuntimeClassInitialize __rctor(c_class)

#define __rdtor(x) __rcat3(x,_,dtor)
#define RuntimeClassDestroy __rdtor(c_class)

#define COM_INTERFACE(__class__) __class__ vfptr; uint32_t __internal_refcount

#define class \
extern const void* const __rcat2(__,__rcat3(c_class,_,vftbl));\
typedef struct c_class c_class;\
extern HRESULT RuntimeClassInitialize();\
extern void RuntimeClassDestroy(c_class* This);\
struct c_class

#define VirtualTable(Interf) static const __rcat2(Interf,Vtbl) vfptr =
#define ENDCLASSDESC const void* const __rcat2(__,__rcat3(c_class,_,vftbl)) = &vfptr;