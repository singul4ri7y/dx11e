#ifndef _DX11E_MOUSE_
#define _DX11E_MOUSE_

#pragma once

#include <MouseEvent.hpp>
#include <queue>

DX11E_START

class DX11E_API Mouse final {
	
	public: 
		Mouse();
		
		Mouse(const Mouse&) = delete;
		
		~Mouse() = default;
	
		void OnLeftPress(const MousePoint);
		void OnLeftRelease(const MousePoint);
		void OnRightPress(const MousePoint);
		void OnRightRelease(const MousePoint);
		void OnMiddlePress(const MousePoint);
		void OnMiddleRelease(const MousePoint);
		void OnLeftDBLClick(const MousePoint);
		void OnRightDBLClick(const MousePoint);
		void OnWheelUp(const MousePoint);
		void OnWheelDown(const MousePoint);
		void OnMouseMove(const MousePoint);
		void OnRawMouseMove(const MousePoint);
		
		void OnMouseEnter();
		void OnMouseLeave();

		void EnableDBLClicks();
		void DisableDBLClicks();

		void Flush();

		bool IsLeftDown() const;
		bool IsRightDown() const;
		bool IsMiddleDown() const;
		bool IsInWindow() const;
		bool DBLClicksEnabled() const;
		
		void EnableRAWInput();
		void DisableRAWInput();
		BOOL IsRAWInputEnabled() const;

		INT GetLastX() const;
		INT GetLastY() const;
		MousePoint GetLastPoint() const;

		BOOL IsEventBufferEmpty() const;
		MouseEvent ReadEvent();
		
		Mouse& operator = (const Mouse&) = delete;
	
	private: 
		std::queue<MouseEvent> m_eventBuffer;
		
		BOOL m_isLeftDown,
			m_isRightDown,
			m_isMiddleDown,
			m_isDblClick,
			m_isInWindow,
			m_isRawInputEnabled;
		
		MousePoint m_lastMousePoint;
	
};

DX11E_END

#endif // !_DX11E_MOUSE_