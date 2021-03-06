#pragma once
#include <Exception.h>
#include <wil\win32_helpers.h>
#include "Mouse.h"
#include "Keyboard.h"
#include <vector>
#include <optional>
#include <span>

class Window
{
	friend class VGraphicsDevice;
	class WindowClass
	{
		WindowClass()noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	public:
		static constexpr const wchar_t* GetName();
		static constexpr HINSTANCE GetInstance();
	private:
		wil::unique_hicon appico;
		HINSTANCE hinst;
		static WindowClass wndclass;
		static constexpr const wchar_t* wndClassName = L"Veritas Direct3D Window";
	};
public:
	Window(uint32_t width, uint32_t height, std::wstring_view name);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
public:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool CursorEnabled() const noexcept;
	void HideCursor() noexcept;
	HWND GetWindowHandle()const noexcept;
public:
	static std::optional<WPARAM> ProcessMessages()noexcept;
private:
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
private:
	uint32_t width, height;
	wil::unique_hwnd hWindow;
	std::vector<BYTE> rawBuffer;
public:
	Mouse mouse;
	Keyboard kbd;
private:
	bool cursorEnabled = true;
};

#define WND_LAST_EXCEPT() HrException(__LINE__, __FILEW__, GetLastError())
#define WND_CALL_INFO(call) if(!(call)) throw(WND_LAST_EXCEPT())