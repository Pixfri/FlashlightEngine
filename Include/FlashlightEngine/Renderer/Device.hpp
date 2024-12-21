// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_DEVICE_HPP
#define FL_RENDERER_DEVICE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>

namespace FlashlightEngine {
    class Device {
    public:
        Device();
        ~Device();

        Device(const Device&) = delete;
        Device(Device&& other) noexcept;

        [[nodiscard]] inline ComPtr<ID3D11Device> GetDevice() const;
        [[nodiscard]] inline ComPtr<ID3D11DeviceContext> GetDeviceContext() const;
        [[nodiscard]] inline ComPtr<IDXGIFactory2> GetDxgiFactory() const;

        [[nodiscard]] inline bool IsValid() const;

        Device& operator=(const Device&) = delete;
        Device& operator=(Device&& other) noexcept;
    
    private:
        ComPtr<ID3D11Device> m_Device{nullptr};

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        ComPtr<ID3D11Debug> m_DebugInterface{nullptr};
#endif

        ComPtr<ID3D11DeviceContext> m_Context{nullptr};
        ComPtr<IDXGIFactory2> m_DxgiFactory{nullptr};
    };
}

#include <FlashlightEngine/Renderer/Device.inl>

#endif // FL_RENDERER_DEVICE_HPP
