// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RENDERER_HPP
#define FL_RENDERER_RENDERER_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/DxUtils.hpp>
#include <FlashlightEngine/Renderer/Enums.hpp>

#include <FlashlightEngine/Core/Window.hpp>

namespace FlashlightEngine {
    class Renderer {
    public:
        Renderer(const std::shared_ptr<Window>& window, bool useWarpAdapter);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;

    private:
        std::shared_ptr<Window> m_Window;

        static UInt32 m_FrameCount;

        bool m_UseWarp{false};

        ComPtr<ID3D12Device2> m_Device{nullptr};
        ComPtr<ID3D12CommandQueue> m_CommandQueue{nullptr};
        ComPtr<IDXGISwapChain4> m_Swapchain{nullptr};
        std::vector<ComPtr<ID3D12Resource>> m_BackBuffers;
        ComPtr<ID3D12GraphicsCommandList> m_CommandList{nullptr};
        std::vector<ComPtr<ID3D12CommandAllocator>> m_CommandAllocators;
        ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap{nullptr};

        UInt32 m_RTVDescriptorSize{0};
        UInt32 m_CurrentBackBufferIndex{0};

        // Synchronization objects
        ComPtr<ID3D12Fence> m_Fence{nullptr};
        UInt64 m_FenceValue{0};
        std::vector<UInt64> m_FrameFenceValues;
        HANDLE m_FenceEvent{};

#if defined(FL_DEBUG) || defined(FL_FORCE_D3D12_DEBUG)
        ComPtr<ID3D12Debug1> m_DebugInterface;
        ComPtr<ID3D12DebugDevice> m_DebugDevice;

        void EnableDebugLayer();
        void SetupInfoQueue();
#endif

        [[nodiscard]] ComPtr<IDXGIAdapter4> GetAdapter() const;
        void CreateDevice(const ComPtr<IDXGIAdapter4>& adapter);
    };
}

#include <FlashlightEngine/Renderer/Renderer.inl>

#endif // FL_RENDERER_RENDERER_HPP
