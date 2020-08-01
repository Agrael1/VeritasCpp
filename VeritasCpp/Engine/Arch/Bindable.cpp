#include "Bindable.h"
#include "Engine\VeritasEngine.h"

IVContext* Bindable::GetContext(VeritasEngine& in)
{
	return in.pContext.Get();
}
IVDevice* Bindable::GetDevice(VeritasEngine& in)
{
	return in.pGfx.Get();
}
IVSwapChain* Bindable::GetSwapChain(VeritasEngine& in)
{
	return in.pSwap.Get();
}