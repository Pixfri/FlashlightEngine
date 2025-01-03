// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Renderer/Wrapper/WGPUContext.hpp>

#include <FlashlightEngine/Renderer/Wrapper/WGPUUtils.hpp>

#include <glfw3webgpu.h>

#include <webgpu/wgpu.h>

namespace FlashlightEngine::FlWGPU {
    WGPUContext::WGPUContext(const Window& window, const bool useHighPerfGPU) {
        CreateInstance();
        CreateSurface(window);
        RequestAdapter(useHighPerfGPU);
        RequestDevice();
    }

    WGPUContext::~WGPUContext() {
        wgpuDeviceRelease(m_Device);
        wgpuAdapterRelease(m_Adapter);
        wgpuSurfaceRelease(m_Surface);
        wgpuInstanceRelease(m_Instance);
    }

    WGPUContext::WGPUContext(WGPUContext&& other) noexcept {
        m_Instance = other.m_Instance;
        m_Surface = other.m_Surface;
        m_Adapter = other.m_Adapter;
        m_Device = other.m_Device;

        other.m_Instance = nullptr;
        other.m_Surface = nullptr;
        other.m_Adapter = nullptr;
        other.m_Device = nullptr;
    }

    WGPUContext& WGPUContext::operator=(WGPUContext&& other) noexcept {
        m_Instance = other.m_Instance;
        m_Surface = other.m_Surface;
        m_Adapter = other.m_Adapter;
        m_Device = other.m_Device;

        other.m_Instance = nullptr;
        other.m_Surface = nullptr;
        other.m_Adapter = nullptr;
        other.m_Device = nullptr;

        return *this;
    }

    void WGPUContext::CreateInstance() {
        WGPUInstanceDescriptor instanceInfo{};
        instanceInfo.nextInChain = nullptr;

        m_Instance = wgpuCreateInstance(&instanceInfo);

        FlAssert(m_Instance != nullptr, "[WebGPU] Failed to create WGPU instance.");

        spdlog::debug("[WebGPU] Successfully created WGPU instance.");
    }

    void WGPUContext::CreateSurface(const Window& window) {
        m_Surface = glfwGetWGPUSurface(m_Instance, window.GetNativeWindow());

        FlAssert(m_Surface != nullptr, "[WebGPU] Failed to create WGPU surface.");

        spdlog::debug("[WebGPU] Successfully created WGPU surface.");
    }

    void WGPUContext::RequestAdapter(const bool useHighPerfGPU) {
        struct UserData {
            WGPUAdapter Adapter = nullptr;
            bool RequestEnded = false;
        } userData;

        // Callback called by wgpuInstanceRequestAdapter when the request returns
        // This is a C++ lambda function, but could be any function defined in the
        // global scope. It must be non-capturing (the brackets [] are empty) so
        // that it behaves like a regular C function pointer, which is what
        // wgpuInstanceRequestAdapter expects (WebGPU being a C API). The workaround
        // is to convey what we want to capture through the pUserData pointer,
        // provided as the last argument of wgpuInstanceRequestAdapter and received
        // by the callback as its last argument.
        auto onAdapterRequestEnded = [](const WGPURequestAdapterStatus status,
                                        WGPUAdapter chosenAdapter,
                                        const char* pMessage,
                                        void* pUserData) {
            auto& [Adapter, RequestEnded] = *static_cast<UserData*>(pUserData);
            if (status == WGPURequestAdapterStatus_Success) {
                Adapter = chosenAdapter;
            } else {
                spdlog::error("[WebGPU] Failed to request WGPU adapter: {}", pMessage);
            }
            RequestEnded = true;
        };

        WGPURequestAdapterOptions adapterOptions{};
        adapterOptions.compatibleSurface = m_Surface;
        adapterOptions.powerPreference = useHighPerfGPU
                                             ? WGPUPowerPreference_HighPerformance
                                             : WGPUPowerPreference_LowPower;

        wgpuInstanceRequestAdapter(m_Instance, &adapterOptions, onAdapterRequestEnded, &userData);

        FlAssert(userData.RequestEnded, "[WebGPU] Adapter request did not end.");

        m_Adapter = userData.Adapter;

        FlAssert(m_Adapter != nullptr, "[WebGPU] Failed to request WGPU adapter.");

        spdlog::debug("[WebGPU] Successfully requested WGPU adapter.");

        PrintAdapterProperties();
    }

    void WGPUContext::RequestDevice() {
        struct UserData {
            WGPUDevice Device = nullptr;
            bool RequestEnded = false;
        } userData;


        auto onDeviceRequestEnded = [](const WGPURequestDeviceStatus status,
                                       WGPUDevice chosenDevice,
                                       const char* pMessage,
                                       void* pUserData) {
            auto& [Device, RequestEnded] = *static_cast<UserData*>(pUserData);
            if (status == WGPURequestDeviceStatus_Success) {
                Device = chosenDevice;
            } else {
                spdlog::error("[WebGPU] Failed to request WGPU device: {}", pMessage);
            }
            RequestEnded = true;
        };

        WGPUDeviceDescriptor deviceInfo{};
        deviceInfo.nextInChain = nullptr;
        deviceInfo.label = "Device";
        deviceInfo.requiredFeatureCount = 0;
        deviceInfo.requiredFeatures = nullptr;
        deviceInfo.defaultQueue.nextInChain = nullptr;
        deviceInfo.defaultQueue.label = "Main Queue";
        deviceInfo.deviceLostCallback = [](const WGPUDeviceLostReason reason,
                                           const char* pMessage,
                                           void* /*pUserData*/) {
            spdlog::error("[WebGPU] Device lost. Reason: {}.", WGPUDeviceLostReasonToString(reason));
            if (pMessage) {
                spdlog::error("[WebGPU] Message: {}", pMessage);
            }
        };

        wgpuAdapterRequestDevice(m_Adapter, &deviceInfo, onDeviceRequestEnded, &userData);

        FlAssert(userData.RequestEnded, "[WebGPU] Device request did not end.");

        m_Device = userData.Device;

        FlAssert(m_Device != nullptr, "[WebGPU] Failed to request WGPU device.");

        spdlog::debug("[WebGPU] Successfully requested WGPU device.");
    }

    void WGPUContext::PrintAdapterProperties() const {
        spdlog::info("[WebGPU] Adapter properties:");
        WGPUAdapterProperties properties{};
        properties.nextInChain = nullptr;

        wgpuAdapterGetProperties(m_Adapter, &properties);
        if (properties.name) {
            spdlog::info("[WebGPU] \t- Name: {}", properties.name);
        }

        spdlog::info("[WebGPU] \t- Device ID: {}", properties.deviceID);

        if (properties.vendorName) {
            spdlog::info("[WebGPU] \t- Vendor: {}", properties.vendorName);
        }

        spdlog::info("[WebGPU] \t- Vendor ID: {}", properties.vendorID);

        if (properties.driverDescription) {
            spdlog::info("[WebGPU] \t- Device Version: {}", properties.driverDescription);
        }

        spdlog::info("[WebGPU] \t- Device Type: {}", WGPUAdapterTypeToString(properties.adapterType));
        spdlog::info("[WebGPU] \t- Backend Type: {}", WGPUBackendTypeToString(properties.backendType));

        WGPUSupportedLimits supportedLimits{};
        supportedLimits.nextInChain = nullptr;

        if (!wgpuAdapterGetLimits(m_Adapter, &supportedLimits)) {
            spdlog::warn("[WebGPU] Failed to retrieve adapter limits.");
            return;
        }

        spdlog::debug("[WebGPU] Adapter limits:");
        spdlog::debug("[WebGPU] \t- Max Texture Dimension 1D: {}", supportedLimits.limits.maxTextureDimension1D);
        spdlog::debug("[WebGPU] \t- Max Texture Dimension 2D: {}", supportedLimits.limits.maxTextureDimension2D);
        spdlog::debug("[WebGPU] \t- Max Texture Dimension 3D: {}", supportedLimits.limits.maxTextureDimension3D);
        spdlog::debug("[WebGPU] \t- Max Texture Array Layers: {}", supportedLimits.limits.maxTextureArrayLayers);
        spdlog::debug("[WebGPU] \t- Max Bind Groups: {}", supportedLimits.limits.maxBindGroups);
        spdlog::debug("[WebGPU] \t- Max Bind Groups + Vertex Buffers: {}",
                      supportedLimits.limits.maxBindGroups + supportedLimits.limits.maxBindGroupsPlusVertexBuffers);
        spdlog::debug("[WebGPU] \t- Max Bindings Per Bind Group: {}", supportedLimits.limits.maxBindingsPerBindGroup);
        spdlog::debug("[WebGPU] \t- Max Dynamic Uniform Buffers Per Pipeline Layout: {}",
                      supportedLimits.limits.maxDynamicUniformBuffersPerPipelineLayout);
        spdlog::debug("[WebGPU] \t- Max Dynamic Storage Buffers Per Pipeline Layout: {}",
                      supportedLimits.limits.maxDynamicStorageBuffersPerPipelineLayout);
        spdlog::debug("[WebGPU] \t- Max Sampled Textures Per Shader Stage: {}",
                      supportedLimits.limits.maxSampledTexturesPerShaderStage);
        spdlog::debug("[WebGPU] \t- Max Samplers Per Shader Stage: {}",
                      supportedLimits.limits.maxSamplersPerShaderStage);
        spdlog::debug("[WebGPU] \t- Max Storage Buffers Per Shader Stage: {}",
                      supportedLimits.limits.maxStorageBuffersPerShaderStage);
        spdlog::debug("[WebGPU] \t- Max Storage Textures Per Shader Stage: {}",
                      supportedLimits.limits.maxStorageTexturesPerShaderStage);
        spdlog::debug("[WebGPU] \t- Max Uniform Buffers Per Shader Stage: {}",
                      supportedLimits.limits.maxUniformBuffersPerShaderStage);
        spdlog::debug("[WebGPU] \t- Max Uniform Buffer Binding Size: {}",
                      supportedLimits.limits.maxUniformBufferBindingSize);
        spdlog::debug("[WebGPU] \t- Max Storage Buffer Binding Size: {}",
                      supportedLimits.limits.maxStorageBufferBindingSize);
        spdlog::debug("[WebGPU] \t- Min Uniform Buffer Offset Alignment: {}",
                      supportedLimits.limits.minUniformBufferOffsetAlignment);
        spdlog::debug("[WebGPU] \t- Min Storage Buffer Offset Alignment: {}",
                      supportedLimits.limits.minStorageBufferOffsetAlignment);
        spdlog::debug("[WebGPU] \t- Max Vertex Buffers: {}", supportedLimits.limits.maxVertexBuffers);
        spdlog::debug("[WebGPU] \t- Max Buffer Size: {}", supportedLimits.limits.maxBufferSize);
        spdlog::debug("[WebGPU] \t- Max Vertex Attributes: {}", supportedLimits.limits.maxVertexAttributes);
        spdlog::debug("[WebGPU] \t- Max Vertex Buffer Array Stride: {}",
                      supportedLimits.limits.maxVertexBufferArrayStride);
        spdlog::debug("[WebGPU] \t- Max Inter-Stage Shader Components: {}",
                      supportedLimits.limits.maxInterStageShaderComponents);
        spdlog::debug("[WebGPU] \t- Max Inter-Stage Shader Variables: {}",
                      supportedLimits.limits.maxInterStageShaderVariables);
        spdlog::debug("[WebGPU] \t- Max Color Attachments: {}", supportedLimits.limits.maxColorAttachments);
        spdlog::debug("[WebGPU] \t- Max Color Attachment Bytes Per Sample: {}",
                      supportedLimits.limits.maxColorAttachmentBytesPerSample);
        spdlog::debug("[WebGPU] \t- Max Compute Workgroup Storage Size: {}",
                      supportedLimits.limits.maxComputeWorkgroupStorageSize);
        spdlog::debug("[WebGPU] \t- Max Compute Invocations Per Workgroup: {}",
                      supportedLimits.limits.maxComputeInvocationsPerWorkgroup);
        spdlog::debug("[WebGPU] \t- Max Compute Workgroup Size X: {}", supportedLimits.limits.maxComputeWorkgroupSizeX);
        spdlog::debug("[WebGPU] \t- Max Compute Workgroup Size Y: {}", supportedLimits.limits.maxComputeWorkgroupSizeY);
        spdlog::debug("[WebGPU] \t- Max Compute Workgroup Size Z: {}", supportedLimits.limits.maxComputeWorkgroupSizeZ);
        spdlog::debug("[WebGPU] \t- Max Compute Workgroups Per Dimension: {}",
                      supportedLimits.limits.maxComputeWorkgroupsPerDimension);


        const UInt64 featureCount = wgpuAdapterEnumerateFeatures(m_Adapter, nullptr);

        std::vector<WGPUFeatureName> features(featureCount);

        wgpuAdapterEnumerateFeatures(m_Adapter, features.data());

        spdlog::debug("[WebGPU] Adapter features:");
        for (const auto f : features) {
            // Check if the feature is a wgpu-native specific feature, and if it's the case, convert it to the correct
            // enum value. 0x00030001 is the first wgpu-native feature.
            if (EnumAsInteger(f) < 0x00030001) {
                spdlog::debug("[WebGPU] \t- {}", WGPUFeatureToString(f));
            } else {
                spdlog::debug("[WebGPU] \t- {}",
                              WGPUNativeFeatureToString(static_cast<WGPUNativeFeature>(EnumAsInteger(f))));
            }
        }
    }
}
