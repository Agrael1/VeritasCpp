#pragma once
#include <Framework\WinSetup.h>
#include <string>

class Exception
{
public:
	Exception(unsigned int line, const wchar_t* file)noexcept;
public:
	unsigned int GetLine()const noexcept;
	std::wstring_view GetFile()const noexcept;
	std::wstring GetOriginString()const noexcept;
public:
	virtual const wchar_t* GetType()const noexcept;
	virtual const wchar_t* what()const noexcept;
private:
	unsigned int line;
	std::wstring file;
protected:
	mutable std::wstring whatBuffer;
};

class HrException :public Exception
{
public:
	HrException(int line, const wchar_t* file, HRESULT hr);
public:
	const wchar_t* what() const noexcept override;
	const wchar_t* GetType()const noexcept override;
	HRESULT GetErrorCode() const noexcept;
public:
	static std::wstring TranslateErrorCode(HRESULT hr)noexcept;
private:
	HRESULT hResult;
};