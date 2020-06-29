#pragma once
#include <Framework\WinSetup.h>
#include <Framework\Exception.h>
#include <Engine\Mouse.h>
#include <Engine\Keyboard.h>
#include <vector>
#include <optional>


class Window
{
	class WindowException : public Exception
	{
		using Exception::Exception;
	public:
		static std::wstring TranslateErrorCode(HRESULT hr)noexcept;
	};
	class HrException :public WindowException
	{
	public:
		HrException(int line, const wchar_t* file, HRESULT hr);
		const wchar_t* what() const noexcept override;
		const wchar_t* GetType()const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorDescription() const noexcept;
	private:
		HRESULT hResult;
	};
	class NoGfxException :public WindowException
	{
	public:
		using WindowException::WindowException;
		const wchar_t* GetType()const noexcept override;
	};
public:
	Window();
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	void CreateConsole(WORD width, WORD height, BYTE fontw, BYTE fonth);
	void SetConsoleCursor(bool value);
	void SetFont(BYTE fontw, BYTE fonth) const;
	void OutputToScreen(const std::vector<CHAR_INFO>& buffer);
	void SetPalette(COLORREF palette[16]);
public:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool CursorEnabled() const noexcept;
	void HideCursor() noexcept;
	bool InFocus()noexcept;
public:
	HINSTANCE GetInstance()
	{
		return hInst;
	}
public:
	void CatchFocus();
	static std::optional<WPARAM> ProcessMessages()noexcept;
private:
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	void Restore();
private:
	HANDLE hOut;
	HANDLE hIn;

	HANDLE hOriginalConsole;
	HWND hWindow;
	HWND ControlWindow;
	HINSTANCE hInst;

	SMALL_RECT rWindowRect{0};
	COORD cDimentions{0};
	short width = 0, height = 0;

	std::vector<BYTE> rawBuffer;
public:
	Mouse mouse;
	Keyboard kbd;
private:
	bool cursorEnabled = true;
	bool inFocus = true;
private:
	static constexpr LPCWSTR wndClassName = L"Dummy";
};

#define WND_EXCEPT_AUTO() HrException(__LINE__, __FILEW__, GetLastError())
#define WND_CALL_INFO(call) if(!(call)) throw(WND_EXCEPT_AUTO())