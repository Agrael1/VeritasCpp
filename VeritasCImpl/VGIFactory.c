#include "Device.h"

HRESULT __stdcall VFCreateDevice(IVDevice** _out_device, struct IVContext** _out_context)
{
	MakeAndInitialize(_out_device, VGraphicsDevice);
	//RETURN_IF_FAILED(wrl::MakeAndInitialize<VContext>(_out_context));

	return S_OK;
}

