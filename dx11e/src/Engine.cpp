#include <Engine.hpp>
#include <chrono>

DX11E_START

Engine::Engine() : 
	m_window(new Window()),
	m_keyboard(new Keyboard(FALSE, FALSE)),
	m_mouse(new Mouse()),
	m_gfx(new Graphics()),
	m_isRawInputInitialized(FALSE)
{}

Engine::~Engine() {
	delete m_window;
	delete m_keyboard;
	delete m_mouse;
	delete m_gfx;
}

void Engine::Destroy() {
	this -> m_window -> Destroy();
}

BOOL Engine::Update() {
	return this -> m_window -> Broadcast();
}

BOOL Engine::InitializeRawInputDevices() noexcept(FALSE) {
	if(this -> m_isRawInputInitialized == FALSE) {
		RAWINPUTDEVICE* rid = (RAWINPUTDEVICE*) alloca(sizeof(RAWINPUTDEVICE));
		
		rid -> usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid -> usUsage     = HID_USAGE_GENERIC_MOUSE;
		rid -> dwFlags     = NULL;
		rid -> hwndTarget  = this -> m_window -> GetWindowHandle();
		
		if(::RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE)) == FALSE) {
			DX11E_WINDOW_EXCEPTION(L"Failed to initialize RAW INPUT DEVICES.", GetLastError());
			return FALSE;
		}
		
		this -> m_isRawInputInitialized = TRUE;
	}
	
	this -> m_mouse -> EnableRAWInput();
	
	return TRUE;
}

Window* const Engine::GetWindow() const {
	return this -> m_window;
}

Mouse* const Engine::GetMouse() const {
	return this -> m_mouse;
}

Keyboard* const Engine::GetKeyboard() const {
	return this -> m_keyboard;
}

Graphics* const Engine::GetGFX() const {
	return this -> m_gfx;
}

void Engine::Run(Engine& app) {
	if(app.Initialize()) {
		while(app.Update()) 
			app.Render();
	}
}

void Engine::Render() {
	using namespace std::chrono;
	time_point start = steady_clock::now();
	
	this -> m_gfx -> ClearBuffer();
	this -> Draw(this -> m_gfx);
	this -> m_gfx -> Present();
	
	time_point end = steady_clock::now();
	
	duration dur = duration_cast<std::chrono::nanoseconds>(end - start);
	
	FLOAT fps = 10e9 / static_cast<FLOAT>(dur.count());
	
	this -> m_fpscounter.Push(fps);
	
	std::wstring wstr = this -> GetCaptionText();
	wstr += L"                Frame Time: " + std::to_wstring(dur.count() / static_cast<FLOAT>(10e6)) + L" (ms)                FPS: " + std::to_wstring(fps) + L"                Avg. FPS: " + std::to_wstring(this -> m_fpscounter.GetAverageFPS());
	
	::SetWindowTextW(this -> m_window -> GetWindowHandle(), wstr.c_str());
}

LRESULT CALLBACK Engine::m_WindowProcedureEngine(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	WindowDimention windowDimention = this -> m_window -> GetWindowDimention();		// Contains window height and width.
	
	switch(msg) {
		case WM_DESTROY: 
			this -> m_window -> Kill();
			break;
		
		/***** KEYBOARD IMPLEMENTATION *****/
		
		case WM_KEYDOWN: 
			if(!(lParam & 0x40000000) || this -> m_keyboard -> IsKeyAutoRepeat()) 
				this -> m_keyboard -> OnKeyPress(static_cast<const char>(wParam));
			break;

		case WM_CHAR: 
			if(!(lParam & 0x40000000) || this -> m_keyboard -> IsCharAutoRepeat()) 
				this -> m_keyboard -> OnChar(static_cast<const char>(wParam));
			break;

		case WM_KEYUP: 
			this -> m_keyboard -> OnKeyRelease(static_cast<const char>(wParam));
			break;
		
		/***** MOUSE IMPLEMENTATIONS *****/
		
		case WM_LBUTTONDOWN: {
			const POINTS pt = MAKEPOINTS(lParam);
			this -> m_mouse -> OnLeftPress({ pt.x, pt.y });
			break;
		}

		case WM_LBUTTONUP: {
			const POINTS pt = MAKEPOINTS(lParam);
			
			if(pt.x < 0 || pt.x > windowDimention.width || pt.y < 0 || pt.y > windowDimention.height) {
				this -> m_mouse -> OnMouseLeave();
			}
			
			this -> m_mouse -> OnLeftRelease({ pt.x, pt.y });
			break;
		}

		case WM_RBUTTONDOWN: {
			const POINTS pt = MAKEPOINTS(lParam);
			this -> m_mouse -> OnRightPress({ pt.x, pt.y });
			break;
		}

		case WM_RBUTTONUP: {
			const POINTS pt = MAKEPOINTS(lParam);
			this -> m_mouse -> OnRightRelease({ pt.x, pt.y });
			
			if(pt.x < 0 || pt.x > windowDimention.width || pt.y < 0 || pt.y > windowDimention.height) {
				this -> m_mouse -> OnMouseLeave();
			}
			
			break;
		}

		case WM_MBUTTONDOWN: {
			const POINTS pt = MAKEPOINTS(lParam);
			this -> m_mouse -> OnMiddlePress({ pt.x, pt.y });
			break;
		}

		case WM_MBUTTONUP: {
			const POINTS pt = MAKEPOINTS(lParam);
			
			if(pt.x < 0 || pt.x > windowDimention.width || pt.y < 0 || pt.y > windowDimention.height) {
				this -> m_mouse -> OnMouseLeave();
			}
			
			this -> m_mouse -> OnMiddleRelease({ pt.x, pt.y });
			break;
		}

		case WM_MOUSEWHEEL: {
			const POINTS pt = MAKEPOINTS(lParam);

			if(GET_WHEEL_DELTA_WPARAM(wParam) > 0) 
				this -> m_mouse -> OnWheelUp({ pt.x, pt.y });
			else this -> m_mouse -> OnWheelDown({ pt.x, pt.y });
			break;
		}

		case WM_MOUSEMOVE: {
			if(!this -> m_window -> IsCursorEnabled()) {
				if(!this -> m_mouse -> IsInWindow()) {
					this -> m_mouse -> OnMouseEnter();
				}
			}
			
			const POINTS pt = MAKEPOINTS(lParam);
			
			if(pt.x >= 0 && pt.x <= windowDimention.width && pt.y >= 0 && pt.y <= windowDimention.height) {
				this -> m_mouse -> OnMouseMove( { pt.x, pt.y });
				
				if(!this -> m_mouse -> IsInWindow()) {
					this -> m_mouse -> OnMouseEnter();
				}
			} else {
				if(wParam & (MK_LBUTTON | MK_RBUTTON)) {
					this -> m_mouse -> OnMouseMove({ pt.x, pt.y });
				} else {
					this -> m_mouse -> OnMouseLeave();
				}
			}
			break;
		}
		
		case WM_ACTIVATE: {
			// Confine/free cursor on window to foreground/background if cursor disabled.
			if(!this -> m_window -> IsCursorEnabled()) {
				if( wParam & WA_ACTIVE ) {
					this -> m_window -> ConfineCursor();
					this -> m_window -> HideCursor();
				}
			}
			break;
		}
		
		case WM_INPUT: 
			UINT dataSize;

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

			if(dataSize > 0) {
				BYTE* rawdata = new BYTE[dataSize];

				if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata, &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata);

					if(raw -> header.dwType == RIM_TYPEMOUSE) 
						this -> m_mouse -> OnRawMouseMove({ raw -> data.mouse.lLastX, raw -> data.mouse.lLastY });
				}

				delete[] rawdata;
			}
			break;
		
		case WM_LBUTTONDBLCLK: 
			this -> m_mouse -> OnLeftDBLClick({ LOWORD(lParam), HIWORD(lParam) });

			break;

		case WM_RBUTTONDBLCLK:
			this -> m_mouse -> OnRightDBLClick({ LOWORD(lParam), HIWORD(lParam) });

			break;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

DX11E_END