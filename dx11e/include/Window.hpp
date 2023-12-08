#ifndef _DX11E_WINDOW_
#define _DX11E_WINDOW_

#pragma once

#include <DX11Exception.hpp>
#include <resources.hpp>

DX11E_START

class DX11E_API Engine;

class DX11E_API WindowException final : public DX11Exception {
	
	public: 
		WindowException(int, const WCHAR*, const WCHAR*, HRESULT);
		const char* what() const noexcept override;
		const WCHAR* GetType() const;
		HRESULT GetErrorCode() const noexcept;
		const std::wstring GetErrorString() const;

	private:
		HRESULT hr;
};

struct DX11E_API WindowDimention {
	SHORT width,
		height;
};

class DX11E_API Window final {
	
	public: 
		Window();
		
		Window(const Window&) = delete;
		
		Window& operator = (const Window&) = delete;
		
		~Window();

		BOOL Initialize(HINSTANCE, const WCHAR*, const WCHAR*, const WindowDimention, Engine*) noexcept(FALSE);
		void Destroy();

		void Kill() const;
		BOOL Broadcast();

		BOOL IsActive();
		BOOL IsMinimized();
		
		WindowDimention GetWindowDimention() const;
		
		void ShowCursor() const;
		void HideCursor() const;
		void FreeCursor() const;
		void ConfineCursor() const;
		void CursorPosition(const SHORT, const SHORT) const;
		BOOL IsCursorEnabled() const;
		
		void EnableCursor();
		void DisableCursor();
		
		HWND GetWindowHandle() const;

	private: 
		static LRESULT CALLBACK m_WindowProcedure(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);
		static LRESULT CALLBACK m_WindowProcedureRedirect(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);
		
		HWND m_hWnd;
		
		BOOL m_cursorEnabled;

		HINSTANCE m_hInstance;
		const WCHAR* m_className;
		WindowDimention m_windowDimention;
	
};

DX11E_END

#endif // !_DX11E_WINDOW_