#ifndef _DX11E_KEYBOARDEVENT_
#define _DX11E_KEYBOARDEVENT_

#pragma once

#include <dx11ewin.hpp>

DX11E_START

class DX11E_API KeyboardEvent final {
    
    public: 
        enum Events {
            PRESSED = 1, RELEASED, INVALID = -1
        };
    
    public: 
        KeyboardEvent();
        KeyboardEvent(const KeyboardEvent::Events&, const char);

        bool IsPressed() const;
        bool IsReleased() const;
        bool IsInvalid() const;

        char GetKeyCode() const;

    private: 
        const KeyboardEvent::Events m_eventType;
        const char m_key;
    
};

DX11E_END

#endif // !_DX11E_KEYBOARDEVENT_