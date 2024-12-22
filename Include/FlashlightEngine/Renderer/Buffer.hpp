// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_BUFFER_HPP
#define FL_RENDERER_BUFFER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>
#include <FlashlightEngine/Renderer/Device.hpp>

#include <FlashlightEngine/Core/CoreUtils.hpp>

#include <memory>

namespace FlashlightEngine {
    enum class BufferUsage {
        Default = 0,
        Immutable = 1,
        Dynamic = 2,
        Staging = 3
    };

    enum class BufferBindFlags {
        VertexBuffer = Bit(0),
        IndexBuffer = Bit(1),
        ConstantBuffer = Bit(2),
        ShaderResource = Bit(3),
        StreamOutput = Bit(4),
        RenderTarget = Bit(5),
        DepthStencil = Bit(6),
        UnorderedAccess = Bit(7),
        Decoder = Bit(8),
        VideoEncoder = Bit(9)
    };

    inline BufferBindFlags operator|(const BufferBindFlags lhs, const BufferBindFlags rhs) {
        return static_cast<BufferBindFlags>(EnumAsInteger(lhs) | EnumAsInteger(rhs));
    }

    enum class BufferCPUAccess {
        None = 0,

        Write = Bit(0),
        Read = Bit(1)
    };

    inline BufferCPUAccess operator|(const BufferCPUAccess lhs, const BufferCPUAccess rhs) {
        return static_cast<BufferCPUAccess>(EnumAsInteger(lhs) | EnumAsInteger(rhs));
    }

    class Buffer {
    public:
        Buffer(const std::shared_ptr<Device>& device,
               const void* data,
               UInt32 size,
               D3D11_USAGE usage,
               D3D11_BIND_FLAG bindFlags,
               std::string_view name,
               bool enableCpuAccess = false,
               D3D11_CPU_ACCESS_FLAG cpuAccess = D3D11_CPU_ACCESS_WRITE);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer(Buffer&& other) noexcept;

        [[nodiscard]] inline ComPtr<ID3D11Buffer> GetBuffer() const;

        void Allocate(const void* data,
                      UInt32 size,
                      D3D11_USAGE usage,
                      D3D11_BIND_FLAG bindFlags,
                      bool enableCpuAccess,
                      D3D11_CPU_ACCESS_FLAG cpuAccess);
        void Free();

        Buffer& operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&& other) noexcept;

    private:
        ComPtr<ID3D11Buffer> m_Buffer{nullptr};

        std::shared_ptr<Device> m_Device{nullptr};
    };
}

#include <FlashlightEngine/Renderer/Buffer.inl>

#endif // FL_RENDERER_BUFFER_HPP
