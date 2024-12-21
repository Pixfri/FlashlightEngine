// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_EVENTS_HPP
#define FL_CORE_EVENTS_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <functional>
#include <memory>
#include <sstream>
#include <string>

namespace FlashlightEngine {
#define FL_BIND_EVENT(x) [this](auto&& PH1) { return (x)(PH1); }

    enum class EventType {
        Null = 0,
        WindowClose, WindowResize, WindowMoved, WindowFocus,
        KeyDown, KeyUp, KeyTyped,
        AppTick, AppUpdate, AppRender,
        MouseButtonDown, MouseButtonUp, MouseMotion, MouseScrolled
    };

    enum class EventCategory {
        Null = 0,
        Application = Bit(0),
        Input = Bit(1),
        Keyboard = Bit(2),
        Mouse = Bit(3),
        MouseButton = Bit(4)
    };

    inline EventCategory operator|(const EventCategory lhs, const EventCategory rhs) {
        return static_cast<EventCategory>(EnumAsInteger(lhs) | EnumAsInteger(rhs));
    }

    // Creating a bunch of define to easily implement subclasses of the Event class
#define FL_EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define FL_EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return EnumAsInteger(category); }

    class Event {
        friend class EventDispatcher;

    public:
        virtual ~Event() = default;
        [[nodiscard]] virtual inline EventType GetEventType() const = 0;
        [[nodiscard]] virtual inline const char* GetName() const = 0;
        [[nodiscard]] virtual inline int GetCategoryFlags() const = 0;

        [[nodiscard]] virtual inline std::string ToString() const;

        [[nodiscard]] inline bool IsInCategory(EventCategory category) const;

        [[nodiscard]] inline bool IsHandled() const;
        inline void StopPropagation();

    private:
        bool m_Handled = false;
    };

    class EventDispatcher {
    public:
        explicit inline EventDispatcher(Event& event);

        template <typename T>
        bool Dispatch(std::function<void(T&)> func);

    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e);

    // -------------------------------------- Application Events --------------------------------------

    class WindowResizeEvent final : public Event {
    public:
        inline WindowResizeEvent(UInt32 width, UInt32 height);

        ~WindowResizeEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override;

        [[nodiscard]] inline UInt32 GetWidth() const;
        [[nodiscard]] inline UInt32 GetHeight() const;
        [[nodiscard]] inline std::pair<UInt32, UInt32> GetSize() const;

        FL_EVENT_CLASS_TYPE(EventType::WindowResize)
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application)

    private:
        UInt32 m_Width, m_Height;
    };

    class WindowCloseEvent final : public Event {
    public:
        WindowCloseEvent() = default;

        FL_EVENT_CLASS_TYPE(EventType::WindowClose);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class WindowFocusEvent final : public Event {
    public:
        explicit inline WindowFocusEvent(bool hasFocus);

        [[nodiscard]] inline bool IsFocused() const;

        FL_EVENT_CLASS_TYPE(EventType::WindowFocus);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);

    private:
        bool m_HasFocus;
    };

    class WindowMovedEvent final : public Event {
    public:
        explicit inline WindowMovedEvent(Int32 x, Int32 y);

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::WindowMoved);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);

    private:
        Int32 m_X, m_Y;
    };

    class AppTickEvent final : public Event {
    public:
        AppTickEvent() = default;

        FL_EVENT_CLASS_TYPE(EventType::AppTick);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class AppUpdateEvent final : public Event {
    public:
        AppUpdateEvent() = default;

        FL_EVENT_CLASS_TYPE(EventType::AppUpdate);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    class AppRenderEvent final : public Event {
    public:
        AppRenderEvent() = default;

        FL_EVENT_CLASS_TYPE(EventType::AppRender);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Application);
    };

    // -------------------------------------- Keys Events --------------------------------------

    class KeyEvent : public Event {
    public:
        [[nodiscard]] inline Int32 GetKeyCode() const;

        FL_EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input);

    protected:
        explicit inline KeyEvent(Int32 keyCode);

        Int32 m_KeyCode;
    };

    class KeyDownEvent final : public KeyEvent {
    public:
        inline KeyDownEvent(Int32 keyCode, UInt32 repeatCount);

        [[nodiscard]] inline UInt32 GetRepeatCount() const;

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::KeyDown);

    private:
        UInt32 m_RepeatCount;
    };

    class KeyUpEvent final : public KeyEvent {
    public:
        explicit inline KeyUpEvent(Int32 keyCode);

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::KeyUp);
    };

    class KeyTypedEvent final : public KeyEvent {
    public:
        explicit inline KeyTypedEvent(Int32 keyCode);

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::KeyTyped);
    };

    // -------------------------------------- Mouse Events --------------------------------------

    class MouseMotionEvent final : public Event {
    public:
        inline MouseMotionEvent(Float32 x, Float32 y);

        [[nodiscard]] inline Float32 GetX() const;
        [[nodiscard]] inline Float32 GetY() const;
        [[nodiscard]] inline std::pair<Float32, Float32> GetPosition() const;

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::MouseMotion);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);

    private:
        Float32 m_X, m_Y;
    };

    class MouseScrolledEvent final : public Event {
    public:
        inline MouseScrolledEvent(Float32 xOffset, Float32 yOffset);

        [[nodiscard]] inline Float32 GetXOffset() const;
        [[nodiscard]] inline Float32 GetYOffset() const;

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::MouseScrolled);
        FL_EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);

    private:
        Float32 m_XOffset, m_YOffset;
    };

    class MouseButtonEvent : public Event {
    public:
        [[nodiscard]] inline Int32 GetMouseButton() const;

        FL_EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input);

    protected:
        explicit inline MouseButtonEvent(Int32 button);

        Int32 m_Button;
    };

    class MouseButtonDownEvent final : public MouseButtonEvent {
    public:
        explicit inline MouseButtonDownEvent(Int32 button);

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::MouseButtonDown);
    };

    class MouseButtonUpEvent final : public MouseButtonEvent {
    public:
        explicit inline MouseButtonUpEvent(Int32 button);

        [[nodiscard]] inline std::string ToString() const override;

        FL_EVENT_CLASS_TYPE(EventType::MouseButtonUp);
    };
}

#include <FlashlightEngine/Core/Events.inl>

#endif // FL_CORE_EVENTS_HPP
