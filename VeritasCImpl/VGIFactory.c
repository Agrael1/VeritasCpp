#include "Device.h"
#include "Context.h"

__declspec(dllexport) HRESULT __stdcall VFCreateDevice(IVDevice** _out_device, struct IVContext** _out_context)
{
	MakeAndInitialize(_out_device, VGraphicsDevice);
	MakeAndInitialize(_out_context, VContext);

	return S_OK;
}

