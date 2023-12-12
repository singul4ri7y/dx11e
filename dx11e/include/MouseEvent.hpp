#ifndef _DX11E_MOUSEEVENT_
#define _DX11E_MOUSEEVENT_

#pragma once

#include <dx11ewin.hpp>

DX11E_START

struct MousePoint final {
    INT x, y;
};

namespace Events {
    enum Mouse {
        L_PRESSED = 1, L_RELEASED,
        R_PRESSED, R_RELEASED,
        M_PRESSED, M_RELEASED,
        L_DBLCLICK, R_DBLCLICK,
        WHEEL_UP, WHEEL_DOWN,
        MOVE, RAW_MOVE,
        INVALID = -1
    };
};

class DX11E_API MouseEvent final {
    public: 
        enum Events {
            L_PRESSED = 1, L_RELEASED,
            R_PRESSED, R_RELEASED,
            M_PRESSED, M_RELEASED,
            L_DBLCLICK, R_DBLCLICK,
            WHEEL_UP, WHEEL_DOWN,
            MOVE, RAW_MOVE,
            INVALID = -1
        };
    
    public: 
        MouseEvent();
        MouseEvent(const MouseEvent::Events&, const MousePoint&);
        
        BOOL IsInvalid() const;
        
        MouseEvent::Events GetEventType() const;
        MousePoint GetMousePoint() const;
        INT GetMousePositionX() const;
        INT GetMousePositionY() const;
    
    private: 
        const MouseEvent::Events m_eventType;
        const MousePoint m_mousePoint;
    
};

DX11E_END

#endif // !_DX11E_MOUSEEVENT_