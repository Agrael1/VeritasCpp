#pragma once

class Bindable
{
public:
	~Bindable() = default;
	virtual void Bind(class VeritasEngine& in) = 0;
protected:
	static class VContext& GetContext(VeritasEngine& in);
	static class VGraphicsDevice& GetDevice(VeritasEngine& in);
	static class VSwapChain& GetSwapChain(VeritasEngine& in);
};