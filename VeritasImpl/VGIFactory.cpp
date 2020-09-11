#include "SwapChain.h"
#include "Device.h"
#include "Context.h"

HRESULT __stdcall VFCreateDevice(IVDevice** _out_device, IVContext** _out_context)
{
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VGraphicsDevice>(_out_device));
	RETURN_IF_FAILED(wrl::MakeAndInitialize<VContext>(_out_context));

	return S_OK;
}
HRESULT __stdcall VFCreateSwapChain(const VSWAP_CHAIN_DESC* descriptor, IVDevice* device, HWND window, IVSwapChain** _out_swapchain)
{
	return wrl::Make<VSwapChain>(descriptor, device, window).CopyTo(_out_swapchain);
}