// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_SAMPLER_HPP
#define FL_RENDERER_SAMPLER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>
#include <FlashlightEngine/Renderer/Device.hpp>

#include <memory>
#include <string_view>

namespace FlashlightEngine {
    class Sampler {
    public:
        Sampler(const std::shared_ptr<Device>& device,
                D3D11_FILTER filter,
                std::string_view name,
                D3D11_TEXTURE_ADDRESS_MODE addressModeU = D3D11_TEXTURE_ADDRESS_WRAP,
                D3D11_TEXTURE_ADDRESS_MODE addressModeV = D3D11_TEXTURE_ADDRESS_WRAP,
                D3D11_TEXTURE_ADDRESS_MODE addressModeW = D3D11_TEXTURE_ADDRESS_WRAP
        );
        ~Sampler();

        Sampler(const Sampler&) = delete;
        Sampler(Sampler&& other) noexcept;

        [[nodiscard]] inline ComPtr<ID3D11SamplerState> GetSampler();

        void UseSampler(UInt32 slot);

        Sampler& operator=(const Sampler&) = delete;
        Sampler& operator=(Sampler&& other) noexcept;

    private:
        ComPtr<ID3D11SamplerState> m_Sampler;

        std::shared_ptr<Device> m_Device;
    };
}

#include <FlashlightEngine/Renderer/Sampler.inl>

#endif // FL_RENDERER_SAMPLER_HPP
