#pragma once
#include <string>

class Exception
{
public:
	Exception(unsigned int line, const wchar_t* file)noexcept;
public:
	unsigned int GetLine()const noexcept;
	const std::wstring& GetFile()const noexcept;
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