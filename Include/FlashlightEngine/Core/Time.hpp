// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_TIME_HPP
#define FL_CORE_TIME_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    class DeltaTime {
    public:
        inline DeltaTime(Float64 time = 0.016f);
        ~DeltaTime() = default;

        DeltaTime(const DeltaTime&) = default;
        DeltaTime(DeltaTime&&) = default;

        [[nodiscard]] inline Float64 AsSeconds() const;

        [[nodiscard]] inline Float64 AsMilliseconds() const;

        operator double() const;

        DeltaTime& operator=(const DeltaTime&) = default;
        DeltaTime& operator=(DeltaTime&&) = default;

    private:
        Float64 m_Time;
    };

    class Time {
    public:
        Time() = delete;
        ~Time() = delete;

        Time(const Time&) = delete;
        Time(Time&&) = delete;

        inline static DeltaTime GetDeltaTime();
        inline static Float64 GetLastFrameTime();

        Time& operator=(const Time&) = delete;
        Time& operator=(Time&&) = delete;
    
    private:
        friend class Application;

        static DeltaTime m_DeltaTime;
        static Float64 m_LastFrameTime;
    };
}

#include <FlashlightEngine/Core/Time.inl>

#endif // FL_CORE_TIME_HPP
