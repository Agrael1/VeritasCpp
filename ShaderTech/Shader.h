#pragma once
#include <string>
#include <tuple>
#include <span>
#include "ShaderCommon.h"
#include "ShaderMath.h"


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
struct DumbPSOut
{
	uint32_t SV_Target;
};

template<class...>struct types { using type = types;};
template<class out, class...Args>struct xtypes { using type = types<Args...>; using Out_t = out; };
template<class Sig> struct args;

template<class Out, class M, class...Args>
struct args<Out(M::*)(Args...)> :xtypes<Out, Args...> {};

template<class Sig> using args_t = typename args<Sig>::type;
template<class Sig> using out_t = typename args<Sig>::Out_t;

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
constexpr std::tuple<Formats...> as_tuple(types<Formats...>, std::span<const dx::XMVECTOR, N> arr, SV_VertexID vid = 0)
{
	return as_tuple<Formats...>(arr, vid, std::make_index_sequence<sizeof...(Formats)>{});
}


struct VertexShaderBase : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVShader>
{
	virtual void __stdcall UpdateConstants(void* const* constants)override;
	virtual void Invoke(const DumbVertex& in, DumbVSOut& out) = 0;
	virtual void __stdcall Invoke(const void* vs_in, void* _out_vertex)override;
	virtual void __stdcall GetByteCode(const char** _out_bytecode)override;
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
		using RQVSOutT = out_t<decltype(&T::main)>;

		RQVSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.data), in.SV_VertexID)));


		if constexpr (sizeof(RQVSOutT) == sizeof(float4A))
		{
			out.SV_PosCoord = 0;
			std::copy((char*)&x, (char*)&x + sizeof(x), (char*)&out.attributes);
		}
		else
		{
			static_assert(alignof(decltype(RQVSOutT::SV_Position)) == alignof(float4A));
			static_assert(sizeof(RQVSOutT::SV_Position) == sizeof(float4A));

			out.SV_PosCoord = offsetof(RQVSOutT, SV_Position) / 16;
			std::copy((char*)&x, (char*)&x + sizeof(x), (char*)&out.attributes);
		}
	}
	void __stdcall GetMonotonicSize(uint32_t* _out_vsize)override
	{
		using RQVSOutT = out_t<decltype(&T::main)>;
		*_out_vsize = sizeof(RQVSOutT) / 16;
	}
};


struct PixelShaderBase : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVShader>
{
	virtual void __stdcall UpdateConstants(void* const* constants)override;
	virtual void Invoke(const DumbVSOut& in, DumbPSOut& out) = 0;
	virtual void __stdcall Invoke(const void* vs_in, void* _out_vertex)override;
	virtual void __stdcall GetByteCode(const char** _out_bytecode)override;
protected:
	std::string ByteCode;
};

template<class T>
struct PixelShader : public PixelShaderBase
{
	PixelShader()
	{
		MakeBytecode(args_t<decltype(&T::main)>{}, ByteCode);
	}
	void Invoke(const DumbVSOut& in, DumbPSOut& out)override
	{
		using RQPSOutT = out_t<decltype(&T::main)>;
		static_assert(sizeof(RQPSOutT) == sizeof(float4));

		RQPSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.attributes))));

		dx::PackedVector::XMStoreColor((dx::PackedVector::XMCOLOR*)&out.SV_Target, x );
	}
	constexpr virtual void __stdcall GetMonotonicSize(uint32_t* _out_vsize)override
	{
		*_out_vsize = 1; //hardcoded for now
	}
};

#undef max
#undef min