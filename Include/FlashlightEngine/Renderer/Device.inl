// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline ComPtr<ID3D11Device> Device::GetDevice() const {
        return m_Device;
    }

    inline ComPtr<ID3D11DeviceContext> Device::GetDeviceContext() const {
        return m_Context;
    }

    inline ComPtr<IDXGIFactory2> Device::GetDxgiFactory() const {
        return m_DxgiFactory;
    }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
    inline bool Device::IsValid() const {
        return m_Device != nullptr && m_DebugInterface != nullptr && m_Context != nullptr && m_DxgiFactory != nullptr;
    }
#else
    inline bool Device::IsValid() const {
        return m_Device != nullptr && m_Context != nullptr && m_DxgiFactory != nullptr;
    }
#endif
}
