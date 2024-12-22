// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Texture.hpp>

#include <FlashlightEngine/Core/Filesystem.hpp>

#include <FreeImage.h>
#include <DirectXTex.h>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    Texture::Texture(const std::filesystem::path& path,
                     const std::string_view name,
                     const std::shared_ptr<Device>& device)
        : m_Device(device) {
        if (path.extension().string() == ".dds") {
            CreateTextureViewFromDDS(path, name);
        } else {
            CreateTextureView(path, name);
        }

        if (m_TextureSrv == nullptr) {
            spdlog::error("[DirectX] Failed to create texture from file: {}", path.string());
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        const std::string srvName = std::string(name) + " SRV";
        if (const HRESULT hr = m_TextureSrv->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UInt32>(srvName.size()), srvName.data());
            FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set debug name for texture: {}. Error: {}", name, HResultToString(hr));
        }
#endif
    }

    Texture::~Texture() {
        m_TextureSrv.Reset();
    }

    Texture::Texture(Texture&& other) noexcept {
        m_TextureSrv.Swap(other.m_TextureSrv);

        m_Device = other.m_Device;
        other.m_Device = nullptr;
    }

    void Texture::UseTexture(const UInt32 slot) {
        if (m_TextureSrv != nullptr) {
            m_Device->GetDeviceContext()->PSSetShaderResources(slot, 1, m_TextureSrv.GetAddressOf());
        }
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        m_TextureSrv.Swap(other.m_TextureSrv);

        m_Device = other.m_Device;
        other.m_Device = nullptr;

        return *this;
    }

    void Texture::CreateTextureViewFromDDS(const std::filesystem::path& path, std::string_view name) {
        DirectX::TexMetadata metadata{};
        DirectX::ScratchImage scratchImage;

        if (!Filesystem::Exists(path)) {
            spdlog::error("[DirectXTex] Texture file does not exist: {}", path.string());
            return;
        }

        HRESULT hr = DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImage);

        if (FAILED(hr)) {
            spdlog::error("[DirectXTex] Failed to load texture from DDS file: {}. Error: {}", path.string(),
                          HResultToString(hr));
            return;
        }

        ComPtr<ID3D11Resource> texture = nullptr;
        hr = DirectX::CreateTexture(m_Device->GetDevice().Get(), scratchImage.GetImages(), scratchImage.GetImageCount(),
                                    metadata, &texture);
        if (FAILED(hr)) {
            spdlog::error("[DirectXTex] Failed to create texture from DDS file: {}. Error: {}", path.string(),
                          HResultToString(hr));
            scratchImage.Release();
            return;
        }

        ID3D11ShaderResourceView* srv = nullptr;

        hr = DirectX::CreateShaderResourceView(m_Device->GetDevice().Get(), scratchImage.GetImages(),
                                               scratchImage.GetImageCount(), metadata, &srv);
        if (FAILED(hr)) {
            spdlog::error("[DirectXTex] Failed to create shader resource view from DDS file: {}. Error: {}",
                          path.string(), HResultToString(hr));
            scratchImage.Release();
            return;
        }

        m_TextureSrv = srv;
    }

    void Texture::CreateTextureView(const std::filesystem::path& path, std::string_view name) {
        HRESULT hr = S_OK;

        FIBITMAP* image = nullptr;
        // Win32 methods for opening files is called "CreateFile" counterintuitively, we make sure to tell it to only read pre-existing files

        //We open a new local scope here so we don't keep the vector in memory for the entire function call, we can get rid of the memory it holds earlier this way
        {
            std::vector<char> fileDataRaw = Filesystem::ReadBytes(path);

            FIMEMORY* memHandle = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(fileDataRaw.data()),
                                                       static_cast<DWORD>(fileDataRaw.size()));
            FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileTypeFromMemory(memHandle);
            if (imageFormat == FIF_UNKNOWN) {
                FreeImage_CloseMemory(memHandle);
                spdlog::error("Failed to determine image format: {}", path.string());
                return;
            }
            image = FreeImage_LoadFromMemory(imageFormat, memHandle);

            FreeImage_CloseMemory(memHandle);
        }

        FreeImage_FlipVertical(image);

        UInt32 textureWidth = FreeImage_GetWidth(image);
        UInt32 textureHeight = FreeImage_GetHeight(image);
        UInt32 textureBpp = FreeImage_GetBPP(image);

        D3D11_TEXTURE2D_DESC textureDesc{};
        D3D11_SUBRESOURCE_DATA initialData{};
        ComPtr<ID3D11Texture2D> texture{nullptr};

        DXGI_FORMAT textureFormat = DXGI_FORMAT_UNKNOWN;
        switch (textureBpp) {
        case 8:
            textureFormat = DXGI_FORMAT_R8_UNORM;
            break;
        case 16:
            textureFormat = DXGI_FORMAT_R8G8_UNORM;
            break;
        case 24:
            // D3D11 does not support 24 bit formats for textures, we'll need to convert
            {
                textureBpp = 32;
                FIBITMAP* newImage = FreeImage_ConvertTo32Bits(image);
                FreeImage_Unload(image);
                image = newImage;
                textureFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
            }
            break;
        case 32:
            textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        default:
            {
                //we could try to handle some weird bit count, but these will probably be HDR or some antique format, just exit instead.
                spdlog::warn("Texture {} has nontrivial bits per pixel ({}).", path.string(), textureBpp);
            }
            break;
        }

        textureDesc.Format = textureFormat;
        textureDesc.ArraySize = 1;
        textureDesc.MipLevels = 1;
        textureDesc.Width = textureWidth;
        textureDesc.Height = textureHeight;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        // Populate initial data
        initialData.pSysMem = FreeImage_GetBits(image);
        initialData.SysMemPitch = textureWidth * (textureBpp / 8);

        hr = m_Device->GetDevice()->CreateTexture2D(&textureDesc, &initialData, texture.GetAddressOf());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create texture from file: {}. Error: {}", path.string(),
                          HResultToString(hr));
            FreeImage_Unload(image);
            return;
        }

#if defined(FL_DEBUG) || defined(FL_FORCE_DX_DEBUG_INTERFACE)
        hr = texture->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UInt32>(name.size()), name.data());

        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to set debug name for texture: {}. Error: {}", name, HResultToString(hr));
        }
#endif

        FreeImage_Unload(image);

        ID3D11ShaderResourceView* srv = nullptr;
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Format = textureFormat;
        srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

        hr = m_Device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, &srv);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create shader resource view from file: {}. Error: {}", path.string(),
                          HResultToString(hr));
            return;
        }

        m_TextureSrv = srv;
    }
}
