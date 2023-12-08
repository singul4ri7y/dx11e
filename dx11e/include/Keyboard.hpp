#ifndef _DX11E_KEYBOARD_
#define _DX11E_KEYBOARD_

#pragma once

#include <KeyboardEvent.hpp>
#include <bitset>
#include <queue>

DX11E_START

class DX11E_API Keyboard final {
	
	public: 
		Keyboard();
		
		Keyboard(_In_ const BOOL&, _In_ const BOOL&);
		
		Keyboard(const Keyboard&) = delete;
		
		~Keyboard() = default;
		
		Keyboard& operator = (const Keyboard&) = delete;

		BOOL IsPressed(const char&) const;

		BOOL IsKeyBufferEmpty() const;
		BOOL IsCharBufferEmpty() const;

		KeyboardEvent ReadKey();
		const char ReadChar();

		void OnKeyPress(const char&);
		void OnKeyRelease(const char&);
		void OnChar(const char);

		void EnableKeyAutoRepeat();
		void DisableKeyAutoRepeat();

		void EnableCharAutoRepeat();
		void DisableCharAutoRepeat();

		BOOL IsKeyAutoRepeat() const;
		BOOL IsCharAutoRepeat() const;

		void FlushKeyBuffer();
		void FlushCharBuffer();
		void FlushBuffers();
		
	public: 
		inline static constexpr unsigned short MAX_KEYS = 256u;
		
	private: 
		BOOL m_isKeyAutoRepeat,
		     m_isCharAutoRepeat;

		std::bitset<Keyboard::MAX_KEYS> m_keyStates;

		std::queue<KeyboardEvent> m_keyBuffer;
		std::queue<char> m_charBuffer;
	
};

DX11E_END

#endif // !_DX11E_KEYBOARD_