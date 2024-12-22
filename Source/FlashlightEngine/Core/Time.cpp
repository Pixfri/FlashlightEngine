// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Time.hpp>

namespace FlashlightEngine {
    DeltaTime Time::m_DeltaTime = DeltaTime{};
    Float64 Time::m_LastFrameTime = {};
}
