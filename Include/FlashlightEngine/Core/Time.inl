// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline DeltaTime::DeltaTime(const Float64 time)
        : m_Time(time) {
    }

    inline Float64 DeltaTime::AsSeconds() const {
        return m_Time;
    }

    inline Float64 DeltaTime::AsMilliseconds() const {
        return m_Time * 1000.0f;
    }

    inline DeltaTime::operator double() const {
        return m_Time;
    }

    inline DeltaTime Time::GetDeltaTime() {
        return m_DeltaTime;
    }

    inline Float64 Time::GetLastFrameTime() {
        return m_LastFrameTime;
    }
}
