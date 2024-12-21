// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Buffer.hpp>

#include <spdlog/spdlog.h>

namespace FlashlightEngine {
    Buffer::Buffer(const std::shared_ptr<Device>& device,
                   const void* data,
                   const UInt32 size,
                   const D3D11_USAGE usage,
                   const D3D11_BIND_FLAG bindFlags,
                   const bool enableCpuAccess,
                   const D3D11_CPU_ACCESS_FLAG cpuAccess)
        : m_Device(device) {
        Allocate(data, size, usage, bindFlags, enableCpuAccess, cpuAccess);
    }

    Buffer::~Buffer() {
        Free();
    }

    Buffer::Buffer(Buffer&& other) noexcept {
        m_Buffer.Swap(other.m_Buffer);
        m_Device = other.m_Device;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        m_Buffer.Swap(other.m_Buffer);
        m_Device = other.m_Device;

        return *this;
    }

    void Buffer::Allocate(const void* data,
                          const UInt32 size,
                          const D3D11_USAGE usage,
                          const D3D11_BIND_FLAG bindFlags,
                          const bool enableCpuAccess,
                          const D3D11_CPU_ACCESS_FLAG cpuAccess) {
        D3D11_BUFFER_DESC bufferInfo{};
        bufferInfo.ByteWidth = size;
        bufferInfo.Usage = usage;
        bufferInfo.BindFlags = bindFlags;
        bufferInfo.CPUAccessFlags = enableCpuAccess ? cpuAccess : 0;

        D3D11_SUBRESOURCE_DATA resourceData{};
        resourceData.pSysMem = data;

        const HRESULT hr = m_Device->GetDevice()->CreateBuffer(&bufferInfo, &resourceData, &m_Buffer);
        if (FAILED(hr)) {
            spdlog::error("[DirectX] Failed to create buffer. Error: {}", HResultToString(hr));
        }
    }


    void Buffer::Free() {
        m_Buffer.Reset();
    }
}
