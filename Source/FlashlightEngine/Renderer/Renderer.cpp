// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Renderer.hpp>

namespace FlashlightEngine {
    UInt32 Renderer::m_FrameCount = 3;

    Renderer::Renderer(const std::shared_ptr<Window>& window, const bool useWarpAdapter)
        : m_Window(window), m_UseWarp(useWarpAdapter), m_BackBuffers(m_FrameCount), m_CommandAllocators(m_FrameCount),
          m_FrameFenceValues(m_FrameCount) {
#if defined(FL_DEBUG) || defined(FL_FORCE_D3D12_DEBUG)
        // We enable it now, since enabling it after creating the device causes a device removed error.
        EnableDebugLayer();
#endif

        const auto adapter = GetAdapter();
        CreateDevice(adapter);

#if defined(FL_DEBUG) || defined(FL_FORCE_D3D12_DEBUG)
        SetupInfoQueue();
#endif
    }

    Renderer::~Renderer() {
    }

#if defined(FL_DEBUG) || defined(FL_FORCE_D3D12_DEBUG)
    void Renderer::EnableDebugLayer() {
        ComPtr<ID3D12Debug> debugInterface;

        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));

        ThrowIfFailed(debugInterface->QueryInterface(IID_PPV_ARGS(&m_DebugInterface)));

        m_DebugInterface->EnableDebugLayer();
        m_DebugInterface->SetEnableGPUBasedValidation(true);
    }

    void Renderer::SetupInfoQueue() {
        ThrowIfFailed(m_Device->QueryInterface(IID_PPV_ARGS(&m_DebugDevice)));

        ComPtr<ID3D12InfoQueue> infoQueue{nullptr};
        ThrowIfFailed(m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue)));

        ThrowIfFailed(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true));
        ThrowIfFailed(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true));
        ThrowIfFailed(infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true));

        D3D12_MESSAGE_SEVERITY suppressSeverities[] = {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        D3D12_MESSAGE_ID suppressIDs[] = {
            // Occurs when an RTV/DSV is cleared with a clear color/depth that was not specified during resource creation.
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
            // Both of these occur when a frame is captured using the integrated Visual Studio debugger.
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
        };

        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumSeverities = ARRAYSIZE(suppressSeverities);
        filter.DenyList.pSeverityList = suppressSeverities;
        filter.DenyList.NumIDs = ARRAYSIZE(suppressIDs);
        filter.DenyList.pIDList = suppressIDs;

        ThrowIfFailed(infoQueue->PushStorageFilter(&filter));
    }
#endif

    ComPtr<IDXGIAdapter4> Renderer::GetAdapter() const {
        ComPtr<IDXGIFactory4> dxgiFactory{nullptr};

        UInt32 createFactoryFlags = 0;

#if defined(FL_DEBUG) || defined(FL_FORCE_D3D12_DEBUG)
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

        ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

        ComPtr<IDXGIAdapter1> dxgiAdapter1{nullptr};
        ComPtr<IDXGIAdapter4> dxgiAdapter4{nullptr};

        if (m_UseWarp) {
            ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
            ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
        } else {
            UInt64 maxDedicatedVideoMemory = 0;

            for (UInt32 i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
                DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
                ThrowIfFailed(dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1));

                // Check to see if the adapter can create a D3D12 device without actually creating it. The adapter with
                // the largest dedicated video memory is favored.
                if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                    SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
                        D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
                    dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory) {
                    maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;

                    ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
                }
            }
        }

        return dxgiAdapter4;
    }

    void Renderer::CreateDevice(const ComPtr<IDXGIAdapter4>& adapter) {
        ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
    }
}
