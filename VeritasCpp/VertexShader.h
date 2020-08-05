#pragma once
#include <string>
#include <tuple>
#include <span>
#include <concepts>
#include <DirectXMath.h>


namespace dx = DirectX;
using matrix = dx::XMMATRIX;
using SV_VertexID = uint32_t;

XM_ALIGNED_STRUCT(16) float1
{
	float1() = default;
	float1(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
	}
	float1 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat(&x, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat(&x);
	}
public:
	float x;
};
struct float2 : public dx::XMFLOAT2A
{
	float2() = default;
	float2(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2A(this, in);
	}
	float2 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat2A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat2A(this);
	}
};
struct float3 : public dx::XMFLOAT3A
{
	float3() = default;
	float3(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3A(this, in);
	}
	float3 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat3A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat3A(this);
	}
};
struct float4 : public dx::XMFLOAT4A
{
	float4() = default;
	float4(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4A(this, in);
	}
	float4 operator=(const dx::XMVECTOR in)
	{
		dx::XMStoreFloat4A(this, in);
		return *this;
	}
	operator dx::XMVECTOR()const
	{
		return dx::XMLoadFloat4A(this);
	}
};

#define float float1

struct DumbVertex
{
	dx::XMVECTOR data[16];
	SV_VertexID SV_VertexID;
};
struct DumbVSOut
{
	dx::XMVECTOR attributes[16];
	uint32_t SV_PosCoord;
};

template<class...>struct types { using type = types; };
template<class Sig> struct args;

template<class Out, class M, class...Args>
struct args<Out(M::*)(Args...)> :types<Args...> {};
template<class Sig> using args_t = typename args<Sig>::type;

template<typename T>
struct map
{
	static constexpr const char* ByteCode = "IN";
	static constexpr uint32_t Stride = 0;
};
template<> struct map<float1>
{
	static constexpr const char* ByteCode = "f1";
	static constexpr uint32_t Stride = sizeof(float1);
};
template<> struct map<float2>
{
	static constexpr const char* ByteCode = "f2";
	static constexpr uint32_t Stride = sizeof(float2);
};
template<> struct map<float3>
{
	static constexpr const char* ByteCode = "f3";
	static constexpr uint32_t Stride = sizeof(float3);
};
template<> struct map<float4>
{
	static constexpr const char* ByteCode = "f4";
	static constexpr uint32_t Stride = sizeof(float4);
};
template<> struct map<SV_VertexID>
{
	static constexpr const char* ByteCode = "";
	static constexpr uint32_t Stride = 0;
};

template <class...Params>
constexpr void MakeBytecode(types<Params...>, std::string& in)
{
	in.reserve(sizeof...(Params) * 2);
	in = (in + ... + map<Params>::ByteCode);
}
template<class T, class C>
constexpr T determine(const dx::XMVECTOR target, SV_VertexID vid)
{
	if constexpr (std::is_same_v<T, C>)
	{
		return vid;
	}
	else
	{
		return target;
	}
}
template <class... Formats, size_t N, size_t... Is>
constexpr std::tuple<Formats...> as_tuple(std::span<const dx::XMVECTOR, N> arr, SV_VertexID vid,
	std::index_sequence<Is...>)
{
	return std::make_tuple(std::forward<Formats>(determine<Formats, SV_VertexID>(arr[Is], vid))...);
}
template <class... Formats, size_t N>
constexpr std::tuple<Formats...> as_tuple(types<Formats...>, std::span<const dx::XMVECTOR, N> arr, SV_VertexID vid)
{
	return as_tuple<Formats...>(arr, vid, std::make_index_sequence<sizeof...(Formats)>{});
}



struct VertexShaderBase
{
	virtual ~VertexShaderBase() = default;
	virtual void UpdateConstants(std::span<uint8_t> constants) {};
	virtual void Invoke(const DumbVertex& in, DumbVSOut& out) = 0;
protected:
	std::string ByteCode;
};

template<class T>
struct VertexShader : public VertexShaderBase
{
	VertexShader()
	{
		MakeBytecode(args_t<decltype(&T::main)>{}, ByteCode);
	}
	void Invoke(const DumbVertex& in, DumbVSOut& out)override
	{
		static_assert(sizeof(T::RQVSOutT::SV_Position) == sizeof(float4));

		typename T::RQVSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.data), in.SV_VertexID)));

		out.SV_PosCoord = offsetof(typename T::RQVSOutT, SV_Position) / 16;
		std::copy((char*)&x, (char*)&x + sizeof(x), (char*)&out.attributes);
	}

};