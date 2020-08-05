#pragma once
#include <Interfaces.h>

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind(class VeritasEngine& in) = 0;
protected:
	static IVContext* GetContext(VeritasEngine& in);
	static IVDevice* GetDevice(VeritasEngine& in);
	static IVSwapChain* GetSwapChain(VeritasEngine& in);
};