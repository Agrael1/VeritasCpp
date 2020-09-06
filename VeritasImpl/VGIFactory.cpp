#include "SwapChain.h"
#include "Device.h"
#include "Context.h"

HRESULT __stdcall VFCreateDevice(HWND window, IVDevice** _out_device, IVContext** _out_context)
{
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VGraphicsDevice>(_out_device, window));
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VContext>(_out_context));

	return S_OK;
}
HRESULT __stdcall VFCreateSwapChain(VSWAP_CHAIN_DESC* descriptor, IVDevice* device, IVSwapChain** _out_swapchain)
{
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VSwapChain>(_out_swapchain, descriptor, device));
	return S_OK;
}