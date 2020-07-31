#include "Bindable.h"
#include "Engine\VeritasEngine.h"

inline VContext& Bindable::GetContext(VeritasEngine& in)
{
	return in.context;
}

inline VGraphicsDevice& Bindable::GetDevice(VeritasEngine& in)
{
	return in.gfx;
}

inline VSwapChain& Bindable::GetSwapChain(VeritasEngine& in)
{
	return in.swap;
}
