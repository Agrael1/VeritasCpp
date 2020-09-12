#include <wil/result_macros.h>
#include "Device.h"
#include "Context.h"

HRESULT __stdcall VFCreateDevice(IVDevice** _out_device, IVContext** _out_context)
{
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VGraphicsDevice>(_out_device));
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VContext>(_out_context));

	return S_OK;
}