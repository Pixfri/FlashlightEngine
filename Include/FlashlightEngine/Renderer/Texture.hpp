// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_TEXTURE_HPP
#define FL_RENDERER_TEXTURE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Device.hpp>
#include <FlashlightEngine/Renderer/Sampler.hpp>

#include <filesystem>
#include <memory>

namespace FlashlightEngine {
    class Texture {
    public:
        Texture(const std::filesystem::path& path,
                std::string_view name,
                const std::shared_ptr<Device>& device);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture(Texture&& other) noexcept;

        [[nodiscard]] inline ComPtr<ID3D11ShaderResourceView> GetTextureSRV() const;

        void UseTexture(UInt32 slot);

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&& other) noexcept;

    private:
        ComPtr<ID3D11ShaderResourceView> m_TextureSrv{nullptr};

        std::shared_ptr<Device> m_Device{nullptr};

        void CreateTextureViewFromDDS(const std::filesystem::path& path, std::string_view name);
        void CreateTextureView(const std::filesystem::path& path, std::string_view name);
    };
}

#include <FlashlightEngine/Renderer/Texture.inl>

#endif // FL_RENDERER_TEXTURE_HPP
