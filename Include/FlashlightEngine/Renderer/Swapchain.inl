// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline ComPtr<IDXGISwapChain1> Swapchain::GetSwapchain() const {
        return m_Swapchain;
    }

    inline ComPtr<ID3D11RenderTargetView> Swapchain::GetRTV() const {
        return m_RenderTargetView;
    }
}
