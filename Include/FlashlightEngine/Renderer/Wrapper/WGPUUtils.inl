// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace FlashlightEngine {
    inline std::string WGPUAdapterTypeToString(const WGPUAdapterType adapterType) {
        switch (adapterType) {
        case WGPUAdapterType_DiscreteGPU:
            return "Discrete (Dedicated) GPU";
        case WGPUAdapterType_CPU:
            return "GPU Emulation (CPU)";
        case WGPUAdapterType_IntegratedGPU:
            return "Integrated GPU";
        case WGPUAdapterType_Unknown:
        default:
            return "Unknown";
        }
    }

    inline std::string WGPUBackendTypeToString(const WGPUBackendType backendType) {
        switch (backendType) {
        case WGPUBackendType_Metal:
            return "Metal";
        case WGPUBackendType_Vulkan:
            return "Vulkan";
        case WGPUBackendType_D3D11:
            return "Direct3D 11";
        case WGPUBackendType_D3D12:
            return "Direct3D 12";
        case WGPUBackendType_OpenGL:
            return "OpenGL";
        case WGPUBackendType_WebGPU:
            return "WebGPU";
        case WGPUBackendType_OpenGLES:
            return "OpenGL ES";
        default:
            return "Unknown";
        }
    }

    inline std::string WGPUFeatureToString(const WGPUFeatureName feature) {
        switch (feature) {
        case WGPUFeatureName_DepthClipControl:
            return "Depth Clip Control";
        case WGPUFeatureName_Depth32FloatStencil8:
            return "Depth 32 Float Stencil 8";
        case WGPUFeatureName_TimestampQuery:
            return "Timestamp Query";
        case WGPUFeatureName_TextureCompressionBC:
            return "Texture Compression BC";
        case WGPUFeatureName_TextureCompressionETC2:
            return "Texture Compression ETC2";
        case WGPUFeatureName_TextureCompressionASTC:
            return "Texture Compression ASTC";
        case WGPUFeatureName_IndirectFirstInstance:
            return "Indirect First Instance";
        case WGPUFeatureName_ShaderF16:
            return "Shader Float 16";
        case WGPUFeatureName_RG11B10UfloatRenderable:
            return "RG11B10 UFloat Renderable";
        case WGPUFeatureName_BGRA8UnormStorage:
            return "BGRA8 UNorm Storage";
        case WGPUFeatureName_Float32Filterable:
            return "Float32 Filterable";
        default:
            return "Unknown feature";
        }
    }

    inline std::string WGPUNativeFeatureToString(const WGPUNativeFeature feature) {
        switch (feature) {
        case WGPUNativeFeature_PushConstants:
            return "Push Constants";
        case WGPUNativeFeature_TextureAdapterSpecificFormatFeatures:
            return "Texture Adapter Specific Format Features";
        case WGPUNativeFeature_MultiDrawIndirect:
            return "Multi Draw Indirect";
        case WGPUNativeFeature_MultiDrawIndirectCount:
            return "Multi Draw Indirect Count";
        case WGPUNativeFeature_VertexWritableStorage:
            return "Vertex Writable Storage";
        case WGPUNativeFeature_TextureBindingArray:
            return "Texture Binding Array";
        case WGPUNativeFeature_SampledTextureAndStorageBufferArrayNonUniformIndexing:
            return "Sampled Texture And Storage Buffer Array Non-Uniform Indexing";
        case WGPUNativeFeature_PipelineStatisticsQuery:
            return "Pipeline Statistics Query";
        case WGPUNativeFeature_StorageResourceBindingArray:
            return "Storage Resource Binding Array";
        case WGPUNativeFeature_PartiallyBoundBindingArray:
            return "Partially Bound Binding Array";
        default:
            return "Unknown native feature";
        }
    }

    inline std::string WGPUDeviceLostReasonToString(const WGPUDeviceLostReason reason) {
        switch (reason) {
        case WGPUDeviceLostReason_Destroyed:
            return "Destroyed";
        default:
            return "Unknown";
        }
    }
}
