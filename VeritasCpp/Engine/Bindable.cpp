#include "VeritasEngine.h"
#include "Bindable.h"


IVContext* Bindable::GetContext(VeritasEngine& in)
{
	return in.pContext.Get();
}
IVDevice* Bindable::GetDevice(VeritasEngine& in)
{
	return in.pDevice.Get();
}
IVSwapChain* Bindable::GetSwapChain(VeritasEngine& in)
{
	return in.pSwap.Get();
}