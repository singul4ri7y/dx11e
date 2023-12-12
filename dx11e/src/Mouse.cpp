#include <Mouse.hpp>

DX11E_START

Mouse::Mouse() : 
    m_isLeftDown(FALSE),
    m_isRightDown(FALSE),
    m_isMiddleDown(FALSE),
    m_isInWindow(FALSE),
    m_isDblClick(FALSE),
    m_isRawInputEnabled(FALSE),
    m_lastMousePoint({ 0, 0 })
{}

void Mouse::OnLeftPress(const MousePoint mousePoint) {
    this -> m_isLeftDown = TRUE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::L_PRESSED, mousePoint));
}

void Mouse::OnLeftRelease(const MousePoint mousePoint) {
    this -> m_isLeftDown = FALSE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::L_RELEASED, mousePoint));
}

void Mouse::OnRightPress(const MousePoint mousePoint) {
    this -> m_isRightDown = TRUE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::R_PRESSED, mousePoint));
}

void Mouse::OnRightRelease(const MousePoint mousePoint) {
    this -> m_isRightDown = FALSE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::R_RELEASED, mousePoint));
}

void Mouse::OnMiddlePress(const MousePoint mousePoint) {
    this -> m_isMiddleDown = TRUE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::M_PRESSED, mousePoint));
}

void Mouse::OnMiddleRelease(const MousePoint mousePoint) {
    this -> m_isMiddleDown = FALSE;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::M_RELEASED, mousePoint));
}

void Mouse::OnLeftDBLClick(const MousePoint mousePoint) {
    this -> m_isLeftDown = FALSE;

    if(this -> m_isDblClick == TRUE) 
        this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::L_DBLCLICK, mousePoint));
}

void Mouse::OnRightDBLClick(const MousePoint mousePoint) {
    this -> m_isRightDown = FALSE;

    if(this -> m_isDblClick == TRUE) 
        this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::R_DBLCLICK, mousePoint));
}

void Mouse::OnWheelUp(const MousePoint mousePoint) {
    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::WHEEL_UP, mousePoint));
}

void Mouse::OnWheelDown(const MousePoint mousePoint) {
    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::WHEEL_DOWN, mousePoint));
}

void Mouse::OnMouseMove(const MousePoint mousePoint) {
    this -> m_lastMousePoint = mousePoint;

    this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::MOVE, mousePoint));
}

void Mouse::OnRawMouseMove(const MousePoint mousePoint) {
    if(this -> IsRAWInputEnabled()) 
        this -> m_eventBuffer.push(MouseEvent(MouseEvent::Events::RAW_MOVE, mousePoint));
}

void Mouse::OnMouseEnter() {
    if(this -> m_isInWindow == FALSE)
        this -> m_isInWindow = TRUE;
}

void Mouse::OnMouseLeave() {
    if(this -> m_isInWindow) 
        this -> m_isInWindow = FALSE;
}

void Mouse::EnableDBLClicks() {
    if(this -> m_isDblClick == FALSE) 
        this -> m_isDblClick = TRUE;
}

void Mouse::DisableDBLClicks() {
    if(this -> m_isDblClick == TRUE) 
        this -> m_isDblClick = FALSE;
}

void Mouse::Flush() {
    this -> m_eventBuffer = std::queue<MouseEvent>();
}

bool Mouse::IsLeftDown() const {
    return this -> m_isLeftDown;
}

bool Mouse::IsRightDown() const {
    return this -> m_isRightDown;
}

bool Mouse::IsMiddleDown() const {
    return this -> m_isMiddleDown;
}

bool Mouse::IsInWindow() const {
    return this -> m_isInWindow;
}

bool Mouse::DBLClicksEnabled() const {
    return this -> m_isDblClick;
}

void Mouse::EnableRAWInput() {
    if(this -> m_isRawInputEnabled == FALSE) 
        this -> m_isRawInputEnabled = TRUE;
}

void Mouse::DisableRAWInput() {
    if(this -> m_isRawInputEnabled == TRUE) 
        this -> m_isRawInputEnabled = FALSE;
}

BOOL Mouse::IsRAWInputEnabled() const {
    return this -> m_isRawInputEnabled;
}

int Mouse::GetLastX() const {
    return this -> m_lastMousePoint.x;
}

int Mouse::GetLastY() const {
    return this -> m_lastMousePoint.y;
}

MousePoint Mouse::GetLastPoint() const {
    return this -> m_lastMousePoint;
}

BOOL Mouse::IsEventBufferEmpty() const {
    return this -> m_eventBuffer.empty();
}

MouseEvent Mouse::ReadEvent() {
    if(!this -> IsEventBufferEmpty()) {
        MouseEvent mouseEvent = this -> m_eventBuffer.front();

        this -> m_eventBuffer.pop();

        return mouseEvent;
    }
    
    return MouseEvent();
}

DX11E_END