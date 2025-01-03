// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_WGPUUTILS_HPP
#define FL_RENDERER_WRAPPER_WGPUUTILS_HPP

#include <FlashlightEngine/Types.hpp>

#include <webgpu/webgpu.h>
#include <webgpu/wgpu.h>

#include <string>

namespace FlashlightEngine {
    inline std::string WGPUAdapterTypeToString(WGPUAdapterType adapterType);
    inline std::string WGPUBackendTypeToString(WGPUBackendType backendType);
    inline std::string WGPUFeatureToString(WGPUFeatureName feature);
    inline std::string WGPUNativeFeatureToString(WGPUNativeFeature feature);
    inline std::string WGPUDeviceLostReasonToString(WGPUDeviceLostReason reason);
}

#include <FlashlightEngine/Renderer/Wrapper/WGPUUtils.inl>

#endif // FL_RENDERER_WRAPPER_WGPUUTILS_HPP
