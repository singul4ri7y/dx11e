#ifndef _DX11E_ENGINE_
#define _DX11E_ENGINE_

#pragma once

#include <Graphics.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <FPSCounter.hpp>
#include <hidusage.h>

DX11E_START

class DX11E_API Engine {
	
	public: 
		Engine();
		virtual ~Engine();

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;

		virtual BOOL Initialize() noexcept(FALSE) = 0;
		virtual void Destroy();
		virtual BOOL Update();
		virtual void Draw(_In_ Graphics* const) = 0;
		void Render();
		BOOL InitializeRawInputDevices() noexcept(FALSE);
		
		Window* const GetWindow() const;
		Mouse* const GetMouse() const;
		Keyboard* const GetKeyboard() const;
		Graphics* const GetGFX() const;
		
		virtual const WCHAR* GetCaptionText() = 0;

		static void Run(_In_ Engine&);
	
	private: 
		LRESULT CALLBACK m_WindowProcedureEngine(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);
	
		Window* const m_window     = nullptr;			// Window.
		Keyboard* const m_keyboard = nullptr;			// Keyboard
		Mouse* const m_mouse       = nullptr;			// Mouse.
		Graphics* const m_gfx      = nullptr;			// Graphics
		
		FPSCounter m_fpscounter;
		
		BOOL m_isRawInputInitialized;
		
	friend class Window;
};

DX11E_END

#endif // !_DX11E_ENGINE_