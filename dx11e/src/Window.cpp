#include <Engine.hpp>

DX11E_START

Window::Window() :
    m_hInstance(nullptr),
    m_className(L"DX11E"),
    m_windowDimention({ 0, 0 }),
    m_cursorEnabled(TRUE)
{}

Window::~Window() {
    this -> Destroy();
}

BOOL Window::Initialize(_In_ HINSTANCE hInstance, _In_ const WCHAR* className, _In_ const WCHAR* windowName, _In_ const WindowDimention windowDimention, _In_ Engine* pEngine) noexcept(FALSE) {
    this -> m_className = className;
    this -> m_windowDimention = windowDimention;

    WNDCLASSEX wndClsEx = { 0 };

    ::ZeroMemory(&wndClsEx, sizeof(WNDCLASSEX));

    wndClsEx.cbSize = sizeof(WNDCLASSEX);

    wndClsEx.cbClsExtra = 0;
    wndClsEx.cbWndExtra = 0;
    wndClsEx.hbrBackground = (HBRUSH) COLOR_BACKGROUND + 5;
    wndClsEx.hInstance = hInstance;
    wndClsEx.lpfnWndProc = Window::m_WindowProcedure;
    wndClsEx.style = CS_GLOBALCLASS | CS_OWNDC | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    wndClsEx.lpszClassName = className;
    wndClsEx.lpszMenuName = 0;

    wndClsEx.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wndClsEx.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(ICON_APPLICATION), IMAGE_ICON, 256, 256, 0);
    wndClsEx.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(ICON_APPLICATION), IMAGE_ICON, 64, 64, 0);

    if(!::RegisterClassEx(&wndClsEx)) {
        DX11E_WINDOW_EXCEPTION(L"FAILED TO REGISTER WINDOW CLASS.", GetLastError());
        return FALSE;
    }

    RECT wr;

    ::ZeroMemory(&wr, sizeof(RECT));

    wr.left = 100;
    wr.right = windowDimention.width + wr.left;
    wr.top = 100;
    wr.bottom = windowDimention.height + wr.left;

    ::AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    this -> m_hWnd = ::CreateWindowEx(
        0,
        className, 
        windowName, 
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        100, 100,
        wr.right - wr.left, wr.bottom - wr.top,
        HWND_DESKTOP,
        0,
        hInstance,
        pEngine
    );

    if(!this -> m_hWnd) {
        DX11E_WINDOW_EXCEPTION(L"FAILED TO INITIALIZE WINDOW.", GetLastError());
        return FALSE;
    }

    ::ShowWindow(this -> m_hWnd, SW_SHOW);
    ::UpdateWindow(this -> m_hWnd);
    ::SetFocus(this -> m_hWnd);
    
    return TRUE;
}

void Window::Destroy() {
    if(::IsWindow(this -> m_hWnd)) {
        ::UnregisterClass(this -> m_className, this -> m_hInstance);
        ::DestroyWindow(this -> m_hWnd);
    }
}

void Window::Kill() const {
    ::PostQuitMessage(0);
}

BOOL Window::Broadcast() {
    MSG msg;

    ::ZeroMemory(&msg, sizeof(msg));

    while(::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);

        if(msg.message == WM_QUIT) {
            this -> Destroy();
            return FALSE;
        }
    }

    ::Sleep(0);

    return TRUE;
}

BOOL Window::IsActive() {
    return ::GetActiveWindow() == this -> m_hWnd;
}

BOOL Window::IsMinimized() {
    return ::IsIconic(this -> m_hWnd);
}

WindowDimention Window::GetWindowDimention() const {
    return this -> m_windowDimention;
}

void Window::FreeCursor() const {
    ::ClipCursor(nullptr);
}

void Window::ConfineCursor() const {
    RECT rect;
    ::GetClientRect(this -> m_hWnd, &rect);
    ::MapWindowPoints(this -> m_hWnd, nullptr, reinterpret_cast<LPPOINT>(&rect), 2);
    ::ClipCursor(&rect);
}

void Window::CursorPosition(const SHORT x, const SHORT y) const {
    ::SetCursorPos(x, y);
}

void Window::ShowCursor() const {
    while(::ShowCursor(TRUE) < 0);
}

void Window::HideCursor() const {
    while(::ShowCursor(FALSE) >= 0);
}

void Window::EnableCursor() {
    this -> m_cursorEnabled = TRUE;
    this -> ShowCursor();
    this -> FreeCursor();
}

void Window::DisableCursor() {
    this -> m_cursorEnabled = FALSE;
    this -> HideCursor();
    this -> ConfineCursor();
}

HWND Window::GetWindowHandle() const {
    return this -> m_hWnd;
}

BOOL Window::IsCursorEnabled() const {
    return this -> m_cursorEnabled;
}

LRESULT CALLBACK Window::m_WindowProcedure(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    switch(msg) {
        case WM_NCCREATE: 
            const CREATESTRUCT* const pCreate = reinterpret_cast<const CREATESTRUCT*>(lParam);
            Engine* const pEngine = reinterpret_cast<Engine* const>(pCreate -> lpCreateParams);
            
            if(pEngine == nullptr) {
                MessageBox(NULL, L"FAILED TO CREATE ENGINE CLASS BEFORE WINDOW INITIALIZATION.", L"DX11E Error", MB_ICONERROR);
                exit(-1);
            }

            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pEngine));
            ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WindowProcedureRedirect));
            break;
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::m_WindowProcedureRedirect(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    Engine* const pEngine = reinterpret_cast<Engine* const>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    return pEngine -> m_WindowProcedureEngine(hWnd, msg, wParam, lParam);
}

/*****     WindowException Implementation     *****/

WindowException::WindowException(int _line, const WCHAR* _file, const WCHAR* _note, HRESULT hr) :
    DX11Exception(_line, _file, _note),
    hr(hr)
{}

const char* WindowException::what() const noexcept {
    std::wostringstream oss;

    oss << this -> GetType() << std::endl
        << L"[ERROR CODE]: " << this -> GetErrorCode() << std::endl
        << L"[DESCRIPTION]: " << this -> GetErrorString()
        << this -> GetSpecificString();

    this -> m_wstringBuffer = oss.str();
    this -> m_stringBuffer = std::string(m_wstringBuffer.begin(), m_wstringBuffer.end());

    return this -> m_stringBuffer.c_str();
}

const WCHAR* WindowException::GetType() const {
    return L"Direct X 11 Window Exception";
}

HRESULT WindowException::GetErrorCode() const noexcept {
    return this -> hr;
}

const std::wstring WindowException::GetErrorString() const {
    return DX11Exception::TranslateErrorCode(this -> hr);
}

DX11E_END