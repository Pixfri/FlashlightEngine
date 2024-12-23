// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace std {
    inline size_t hash<FlashlightEngine::ResourceDescriptor>::operator()(
        const FlashlightEngine::ResourceDescriptor& descriptor) const noexcept {
        constexpr hash<uint64_t> hash;
        return hash(static_cast<uint64_t>(descriptor.SlotIndex) << 32) ^ hash(static_cast<uint32_t>(descriptor.Type));
    }

    inline bool equal_to<FlashlightEngine::ResourceDescriptor>::operator()(
        const FlashlightEngine::ResourceDescriptor& lhs,
        const FlashlightEngine::ResourceDescriptor& rhs) const noexcept {
        return lhs.SlotIndex == rhs.SlotIndex && lhs.Type == rhs.Type;
    }
}
