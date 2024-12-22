// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Device.hpp>

#include <FlashlightEngine/Libs/Windows.hpp>

#include <spdlog/spdlog.h>

#include <array>

namespace FlashlightEngine {
    Device::Device() {
        HRESULT hr = S_OK;

        hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create DXGI factory. Error: {}", HResultToString(hr));
        }

        UInt32 deviceFlags = 0;
#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        constexpr std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };

        D3D_FEATURE_LEVEL preferredFeatureLevel = D3D_FEATURE_LEVEL_11_1;

        constexpr std::array<D3D_FEATURE_LEVEL, 4> featureLevels = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        D3D_DRIVER_TYPE chosenDriverType = D3D_DRIVER_TYPE_UNKNOWN;

        for (UInt32 driverTypeIndex = 0; driverTypeIndex < driverTypes.size(); driverTypeIndex++) {
            chosenDriverType = driverTypes[driverTypeIndex];

            hr = D3D11CreateDevice(nullptr,
                                   chosenDriverType,
                                   nullptr,
                                   deviceFlags,
                                   featureLevels.data(),
                                   static_cast<UInt32>(featureLevels.size()),
                                   D3D11_SDK_VERSION,
                                   &m_Device,
                                   &preferredFeatureLevel,
                                   &m_Context
            );

            if (hr == E_INVALIDARG) {
                spdlog::warn("[DirectX] Cannot create DirectX 11.1 compatible device. Error: {}", HResultToString(hr));

                // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
                hr = D3D11CreateDevice(nullptr,
                                       chosenDriverType,
                                       nullptr,
                                       deviceFlags,
                                       &featureLevels[1],
                                       static_cast<UInt32>(featureLevels.size() - 1),
                                       D3D11_SDK_VERSION,
                                       &m_Device,
                                       &preferredFeatureLevel,
                                       &m_Context
                );
            }

            if (SUCCEEDED(hr)) {
                break;
            }
        }

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create DirectX 11 device. Error: {}", HResultToString(hr));
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_Device.As(&m_DebugInterface);

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create debug interface. Error: {}", HResultToString(hr));
        }
#endif

        spdlog::info("[DirectX] Device created successfully.");

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Device"), "Device");
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set device name. Error: {}", HResultToString(hr));
        }

        hr = m_Context->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Context"), "Context");
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set context name. Error: {}", HResultToString(hr));
        }

        hr = m_DxgiFactory->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DXGI Factory"), "DXGI Factory");
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set DXGI factory name. Error: {}", HResultToString(hr));
        }
#endif
    }

    Device::~Device() {
        m_DxgiFactory.Reset();
        m_Context.Reset();
#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        m_DebugInterface->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
        m_DebugInterface.Reset();
#endif
        m_Device.Reset();

        spdlog::info("[DirectX] Device destroyed.");
    }

    Device::Device(Device&& other) noexcept {
        m_Device.Swap(other.m_Device);

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        m_DebugInterface.Swap(other.m_DebugInterface);
#endif

        m_Context.Swap(other.m_Context);

        m_DxgiFactory.Swap(other.m_DxgiFactory);
    }

    Device& Device::operator=(Device&& other) noexcept {
        m_Device.Swap(other.m_Device);

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        m_DebugInterface.Swap(other.m_DebugInterface);
#endif

        m_Context.Swap(other.m_Context);

        m_DxgiFactory.Swap(other.m_DxgiFactory);

        return *this;
    }
}
