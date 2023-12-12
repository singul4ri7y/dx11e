#include <Keyboard.hpp>

DX11E_START

Keyboard::Keyboard():
    m_isKeyAutoRepeat(FALSE),
    m_isCharAutoRepeat(FALSE)
{}

Keyboard::Keyboard(_In_ const BOOL& isKeyAutoRepeat, _In_ const BOOL& isCharAutoRepeat) : 
    m_isKeyAutoRepeat(isKeyAutoRepeat),
    m_isCharAutoRepeat(isCharAutoRepeat)
{}

BOOL Keyboard::IsPressed(const char& keyCode) const {
    return this -> m_keyStates[keyCode];
}

BOOL Keyboard::IsKeyBufferEmpty() const {
    return this -> m_keyBuffer.empty();
}

BOOL Keyboard::IsCharBufferEmpty() const {
    return this -> m_charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey() {
    if(this -> IsKeyBufferEmpty()) 
        return KeyboardEvent();

    KeyboardEvent keyboardEvent = this -> m_keyBuffer.front();

    // Removing the front keyboard event from key buffer.
    this -> m_keyBuffer.pop();

    return keyboardEvent;
}

const char Keyboard::ReadChar() {
    if(this -> IsCharBufferEmpty()) 
        return 0u;

    char ch = this -> m_charBuffer.front();

    // Removing the char event.
    this -> m_charBuffer.pop();

    return ch;
}

void Keyboard::OnKeyPress(const char& keyCode) {
    this -> m_keyStates[keyCode] = TRUE;

    this -> m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Events::PRESSED, keyCode));
}

void Keyboard::OnKeyRelease(const char& keyCode) {
    this -> m_keyStates[keyCode] = false;

    this -> m_keyBuffer.push(KeyboardEvent(KeyboardEvent::Events::RELEASED, keyCode));
}

void Keyboard::OnChar(const char ch) {
    this -> m_charBuffer.push(ch);
}

void Keyboard::EnableKeyAutoRepeat() {
    if(this -> m_isKeyAutoRepeat == false) 
        this -> m_isKeyAutoRepeat = TRUE;
};

void Keyboard::DisableKeyAutoRepeat() {
    if(this -> m_isKeyAutoRepeat == TRUE) 
        this -> m_isKeyAutoRepeat = false;
};

void Keyboard::EnableCharAutoRepeat() {
    if(this -> m_isCharAutoRepeat == false) 
        this -> m_isCharAutoRepeat = TRUE;
};

void Keyboard::DisableCharAutoRepeat() {
    if(this -> m_isCharAutoRepeat == TRUE) 
        this -> m_isCharAutoRepeat = false;
};

BOOL Keyboard::IsKeyAutoRepeat() const {
    return this -> m_isKeyAutoRepeat;
};

BOOL Keyboard::IsCharAutoRepeat() const {
    return this -> m_isCharAutoRepeat;
}

void Keyboard::FlushKeyBuffer() {
    this -> m_keyBuffer = std::queue<KeyboardEvent>();
}

void Keyboard::FlushCharBuffer() {
    this -> m_keyBuffer = std::queue<KeyboardEvent>();
}

void Keyboard::FlushBuffers() {
    this -> FlushKeyBuffer();
    this -> FlushCharBuffer();
}

DX11E_END