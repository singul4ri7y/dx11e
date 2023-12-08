#include <MouseEvent.hpp>

DX11E_START

MouseEvent::MouseEvent() : 
	m_eventType(MouseEvent::Events::INVALID),
	m_mousePoint({ 0, 0 })
{}


MouseEvent::MouseEvent(const MouseEvent::Events& eventType, const MousePoint& mousePoint) : 
	m_eventType(eventType),
	m_mousePoint(mousePoint)
{}

BOOL MouseEvent::IsInvalid() const {
	return this -> GetEventType() == MouseEvent::Events::INVALID;
}

MouseEvent::Events MouseEvent::GetEventType() const {
	return this -> m_eventType;
}

MousePoint MouseEvent::GetMousePoint() const {
	return this -> m_mousePoint;
}

INT MouseEvent::GetMousePositionX() const {
	return this -> GetMousePoint().x;
}

INT MouseEvent::GetMousePositionY() const {
	return this -> GetMousePoint().y;
}

DX11E_END