#pragma once
#include <Engine\VeritasEngine.h>

class Bindable
{
public:
	~Bindable() = default;
	virtual void Bind(VeritasEngine& in) = 0;
protected:
	static VContext& GetContext(VeritasEngine& in)
	{
		return in.context;
	}
	static VGraphicsDevice& GetDevice(VeritasEngine& in)
	{
		return in.gfx;
	}
	static VSwapChain& GetSwapChain(VeritasEngine& in)
	{
		return in.swap;
	}
};