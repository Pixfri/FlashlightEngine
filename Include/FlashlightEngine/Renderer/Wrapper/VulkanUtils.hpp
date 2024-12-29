// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_VULKANUTILS_HPP
#define FL_RENDERER_WRAPPER_VULKANUTILS_HPP

#include <FlashlightEngine/Types.hpp>

#include <volk.h>

#include <string>

namespace FlashlightEngine {
    inline std::string VkResultToString(VkResult result);
}

#include <FlashlightEngine/Renderer/Wrapper/VulkanUtils.inl>

#endif // FL_RENDERER_WRAPPER_VULKANUTILS_HPP
