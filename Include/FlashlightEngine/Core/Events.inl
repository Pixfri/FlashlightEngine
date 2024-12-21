// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline std::string Event::ToString() const {
        return GetName();
    }

    inline bool Event::IsInCategory(const EventCategory category) const {
        return GetCategoryFlags() & EnumAsInteger(category);
    }

    inline bool Event::IsHandled() const {
        return m_Handled;
    }

    inline void Event::StopPropagation() {
        m_Handled = false;
    }

    inline EventDispatcher::EventDispatcher(Event& event)
        : m_Event(event) {
    }

    template <typename T>
    bool EventDispatcher::Dispatch(std::function<void(T&)> func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            func(static_cast<T&>(m_Event));
            return true;
        }

        return false;
    }

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }


    // -------------------------------------- Application Events --------------------------------------

    inline WindowResizeEvent::WindowResizeEvent(const UInt32 width, const UInt32 height)
        : m_Width(width), m_Height(height) {
    }

    inline std::string WindowResizeEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowResizeEvent(" << m_Width << ", " << m_Height << ")";
        return ss.str();
    }

    inline UInt32 WindowResizeEvent::GetWidth() const {
        return m_Width;
    }

    inline UInt32 WindowResizeEvent::GetHeight() const {
        return m_Height;
    }

    inline std::pair<UInt32, UInt32> WindowResizeEvent::GetSize() const {
        return {m_Width, m_Height};
    }

    inline WindowFocusEvent::WindowFocusEvent(const bool hasFocus)
        : m_HasFocus(hasFocus) {
    }

    inline bool WindowFocusEvent::IsFocused() const {
        return m_HasFocus;
    }

    inline WindowMovedEvent::WindowMovedEvent(const Int32 x, const Int32 y)
        : m_X(x), m_Y(y) {
    }

    inline std::string WindowMovedEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowMovedEvent(" << m_X << ", " << m_Y << ")";
        return ss.str();
    }

    // -------------------------------------- Keys Events --------------------------------------

    inline Int32 KeyEvent::GetKeyCode() const {
        return m_KeyCode;
    }

    inline KeyEvent::KeyEvent(const Int32 keyCode)
        : m_KeyCode(keyCode) {
    }

    inline KeyDownEvent::KeyDownEvent(const Int32 keyCode, const UInt32 repeatCount)
        : KeyEvent(keyCode), m_RepeatCount(repeatCount) {
    }

    inline UInt32 KeyDownEvent::GetRepeatCount() const {
        return m_RepeatCount;
    }

    inline std::string KeyDownEvent::ToString() const {
        std::stringstream ss;
        ss << "KeyDownEvent(" << m_KeyCode << ")";
        return ss.str();
    }

    inline KeyUpEvent::KeyUpEvent(const Int32 keyCode)
        : KeyEvent(keyCode) {
    }

    inline std::string KeyUpEvent::ToString() const {
        std::stringstream ss;
        ss << "KeyUpEvent(" << m_KeyCode << ")";
        return ss.str();
    }

    inline KeyTypedEvent::KeyTypedEvent(const Int32 keyCode)
        : KeyEvent(keyCode) {
    }

    inline std::string KeyTypedEvent::ToString() const {
        std::stringstream ss;
        ss << "KeyTypedEvent(" << m_KeyCode << ")";
        return ss.str();
    }

    // -------------------------------------- Mouse Events --------------------------------------

    inline MouseMotionEvent::MouseMotionEvent(const Float32 x, const Float32 y)
        : m_X(x), m_Y(y) {
    }

    inline Float32 MouseMotionEvent::GetX() const {
        return m_X;
    }

    inline Float32 MouseMotionEvent::GetY() const {
        return m_Y;
    }

    inline std::pair<Float32, Float32> MouseMotionEvent::GetPosition() const {
        return {m_X, m_Y};
    }

    inline std::string MouseMotionEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseMotionEvent(" << m_X << ", " << m_Y << ")";
        return ss.str();
    }

    inline MouseScrolledEvent::MouseScrolledEvent(const Float32 xOffset, const Float32 yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {
    }

    inline Float32 MouseScrolledEvent::GetXOffset() const {
        return m_XOffset;
    }

    inline Float32 MouseScrolledEvent::GetYOffset() const {
        return m_YOffset;
    }

    inline std::string MouseScrolledEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseScrolledEvent(" << m_XOffset << ", " << m_YOffset << ")";
        return ss.str();
    }

    inline Int32 MouseButtonEvent::GetMouseButton() const {
        return m_Button;
    }

    inline MouseButtonEvent::MouseButtonEvent(const Int32 button)
        : m_Button(button) {
    }

    inline MouseButtonDownEvent::MouseButtonDownEvent(const Int32 button)
        : MouseButtonEvent(button) {
    }

    inline std::string MouseButtonDownEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonDownEvent(" << m_Button << ")";
        return ss.str();
    }

    inline MouseButtonUpEvent::MouseButtonUpEvent(const Int32 button)
        : MouseButtonEvent(button) {
    }

    inline std::string MouseButtonUpEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonUpEvent(" << m_Button << ")";
        return ss.str();
    }
}
