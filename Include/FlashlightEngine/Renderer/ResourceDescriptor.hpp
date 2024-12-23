// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_RESOURCEDESCRIPTOR_HPP
#define FL_RENDERER_RESOURCEDESCRIPTOR_HPP

#include <FlashlightEngine/Types.hpp>
#include <FlashlightEngine/Renderer/Enums.hpp>

#include <unordered_map>

namespace FlashlightEngine {
    enum class ResourceType : UInt32 {
        Texture,
        Sampler,
        Buffer
    };

    struct ResourceDescriptor {
        ResourceType Type;
        PipelineBindPoint Stage;
        UInt32 SlotIndex;
    };
}

namespace std {
    template <>
    struct hash<FlashlightEngine::ResourceDescriptor> {
        inline size_t operator()(const FlashlightEngine::ResourceDescriptor& descriptor) const noexcept;
    };

    template <>
    struct equal_to<FlashlightEngine::ResourceDescriptor> {
        inline bool operator()(const FlashlightEngine::ResourceDescriptor& lhs,
                               const FlashlightEngine::ResourceDescriptor& rhs) const noexcept;
    };
}

#include <FlashlightEngine/Renderer/ResourceDescriptor.inl>

#endif // FL_RENDERER_RESOURCEDESCRIPTOR_HPP
