#include <KeyboardEvent.hpp>

DX11E_START

KeyboardEvent::KeyboardEvent(): 
    m_eventType(KeyboardEvent::Events::INVALID),
    m_key(0u)
{}

KeyboardEvent::KeyboardEvent(const KeyboardEvent::Events& evt, const char key): 
    m_eventType(evt),
    m_key(key)
{}

bool KeyboardEvent::IsPressed() const {
    return this -> m_eventType == KeyboardEvent::Events::PRESSED;
}

bool KeyboardEvent::IsReleased() const {
    return this -> m_eventType == KeyboardEvent::Events::RELEASED;
}

bool KeyboardEvent::IsInvalid() const {
    return this -> m_eventType == KeyboardEvent::Events::INVALID;
}

char KeyboardEvent::GetKeyCode() const {
    return this -> m_key;
}

DX11E_END