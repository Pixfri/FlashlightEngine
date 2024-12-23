// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Sampler.hpp>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    Sampler::Sampler(const std::shared_ptr<Device>& device,
                     const D3D11_FILTER filter,
                     const std::string_view name,
                     const D3D11_TEXTURE_ADDRESS_MODE addressModeU,
                     const D3D11_TEXTURE_ADDRESS_MODE addressModeV,
                     const D3D11_TEXTURE_ADDRESS_MODE addressModeW)
        : m_Device(device) {
        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = filter;
        samplerDesc.AddressU = addressModeU;
        samplerDesc.AddressV = addressModeV;
        samplerDesc.AddressW = addressModeW;

        HRESULT hr = device->GetDevice()->CreateSamplerState(&samplerDesc, &m_Sampler);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create sampler state. Error: {0}", HResultToString(hr));
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = m_Sampler->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UInt32>(name.size()), name.data());
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set sampler name. Error: {0}", HResultToString(hr));
        }
#endif
    }

    Sampler::~Sampler() {
        m_Sampler.Reset();
    }

    Sampler::Sampler(Sampler&& other) noexcept {
        m_Sampler.Swap(other.m_Sampler);

        m_Device = other.m_Device;
        other.m_Device = nullptr;
    }

    Sampler& Sampler::operator=(Sampler&& other) noexcept {
        m_Sampler.Swap(other.m_Sampler);

        m_Device = other.m_Device;
        other.m_Device = nullptr;

        return *this;
    }
}
