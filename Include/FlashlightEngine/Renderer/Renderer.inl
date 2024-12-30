// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline FrameData& Renderer::GetCurrentFrame() {
        return m_Frames[m_FrameNumber % g_FramesInFlight];
    }
}
