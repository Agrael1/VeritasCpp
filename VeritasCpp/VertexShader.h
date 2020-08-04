#pragma once
#include <string>
#include <tuple>
#include <span>
#include <DirectXMath.h>

namespace dx = DirectX;
using matrix = dx::XMMATRIX;

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


struct DumbVertex
{
	dx::XMVECTOR data[16];
	uint32_t SV_VertexID;
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
template <class... Formats, size_t N, size_t... Is>
constexpr std::tuple<Formats...> as_tuple(std::span<const dx::XMVECTOR, N> arr,
	std::index_sequence<Is...>)
{
	return std::make_tuple(Formats{ arr[Is] }...);
}
template <class... Formats, size_t N>
constexpr std::tuple<Formats...> as_tuple(types<Formats...>, std::span<const dx::XMVECTOR, N> arr)
{
	return as_tuple<Formats...>(arr, std::make_index_sequence<sizeof...(Formats)>{});
}



struct VertexShader
{
	~VertexShader() = default;
	virtual void UpdateConstants(std::span<uint8_t> constants) {};
	virtual void Invoke(const DumbVertex& in, DumbVSOut& out) = 0;
protected:
	std::string ByteCode;
};

template<class T, class = std::is_base_of<VertexShader, T>>
struct VS : public VertexShader
{
	VS() 
	{
		MakeBytecode(args_t<decltype(&T::main)>{}, ByteCode);
	}
	void Invoke(const DumbVertex& in, DumbVSOut& out)override
	{
		static_assert(sizeof(T::RQVSOutT::SV_Position) == sizeof(float4));

		T::RQVSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.data))));

		out.SV_PosCoord = offsetof(T::RQVSOutT, SV_Position) / 16;
		std::copy((char*)&x, (char*)&x + sizeof(x), (char*)&out.attributes);
	}

};


class SimpleVS : public VS<SimpleVS>
{
public:
	struct CBuffer
	{
		matrix modelView;
		matrix modelViewProj;
	}cbuf;

	struct VSOut
	{
		float3 viewPos; // Position;
		float3 viewNormal; // Normal;
		float2 tc; // Texcoord;
		float4 SV_Position; // SV_Position;
	};
public:
	VSOut main(float3 pos, float3 Normal, float2 Texcoord)
	{
		VSOut vso;
		vso.viewPos = dx::XMVector3Transform(pos, cbuf.modelView);
		vso.viewNormal = dx::XMVector3TransformNormal(Normal, cbuf.modelView);
		vso.SV_Position = dx::XMVector3Transform(pos, cbuf.modelViewProj);
		vso.tc = Texcoord;
		return vso;
	}
public:
	using RQVSOutT = VSOut;
	void UpdateConstants(std::span<uint8_t> constants)override
	{
		std::copy(constants.begin(), constants.end(), (uint8_t*)&cbuf);
	};
};



