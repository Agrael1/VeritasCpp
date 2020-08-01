#pragma once
#include <string>
#include <DirectXMath.h>

namespace dx = DirectX;
using float4 = dx::XMFLOAT4A;
struct DumbVertex
{
	float4 data[16];
};
template<class...>struct types { using type = types; };
template<class Sig> struct args;
template<class R, class...Args>
struct args<R(Args...)> :types<Args...> {};
template<class Sig> using args_t = typename args<Sig>::type;

template<typename T>
struct map 
{
	static constexpr const char* ByteCode = "IN";
	static constexpr uint32_t Stride = 0;
};

template<> struct map<float>
{
	static constexpr const char* ByteCode = "f1";
	static constexpr uint32_t Stride = sizeof(float);
};

template <class...Params>
constexpr void MakeBytecode(types<Params...>, std::string& in)
{
	in.reserve(sizeof...(Params) * 2);
	in = (in + ... + map<Params>::ByteCode);
}


struct VertexShader
{
	~VertexShader() = default;
	virtual void Invoke(DumbVertex) = 0;
protected:
	std::string ByteCode;
};



inline float x(float4 worldPos, float y)
{
	return 0;
}


class SimpleVS : public VertexShader
{
public:
	SimpleVS()noexcept
	{
		MakeBytecode(args_t<decltype(x)>{}, ByteCode);
	}
public:
	void Invoke(DumbVertex xt)override
	{
		x(xt.data[0], xt.data[1].x);
	}
};



