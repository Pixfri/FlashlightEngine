// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline ComPtr<ID3D11SamplerState> Sampler::GetSampler() {
        return m_Sampler;
    }
}