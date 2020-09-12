#pragma once
#include <Exception.h>
#include <string>

import "GDISetup.h";

class GdiException : public Exception
{
public:
	GdiException(Gdiplus::Status in_status, unsigned line, const wchar_t* file)noexcept;
public:
	static const wchar_t* GetStatusDescription(Gdiplus::Status status)noexcept;
	const wchar_t* GetType()const noexcept override;
	const wchar_t* what()const noexcept override;
private:
	Gdiplus::Status status;
};

#ifdef _DEBUG
#define GDI_CALL_EXCEPT(call) if(Gdiplus::Status xstatus = (call); xstatus!=Gdiplus::Ok) throw GdiException{xstatus, (unsigned) __LINE__, __FILEW__}
#else
#define GDI_CALL_EXCEPT(call) call
#endif // _DEBUG


