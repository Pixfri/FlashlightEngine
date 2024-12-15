// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_VULKANRHI_ENUMS_HPP
#define FL_VULKANRHI_ENUMS_HPP

#include <FlashlightEngine/Types.hpp>

#include <type_traits>

namespace FlashlightEngine::Vk {
    enum class CommandPoolType {
        Graphics,
        Compute,
        Transfer
    };

    enum class PipelineType {
        Graphics = 0,
        Compute = 1,

        MaxEnum = 0x7FFFFFFF // Just here to force the enum to be 32-bits
    };

    enum class IndexType {
        UInt16 = 0,
        UInt32 = 1
    };

    // From https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html#commandbuffers-lifecycle
    enum class CommandBufferState {
        Initial, // After allocate or reset
        Recording, // After vkBeginCommandBuffer
        Executable, // After vkEndCommandBuffer or after being executed for non-one-time submit
        Pending, // After submission
        Invalid // After being invalidated or submitted for one-time submit command buffers
    };

    enum class PipelineStageBits {
        TopOfPipe = 0x00000001,
        DrawIndirect = 0x00000002,
        VertexInput = 0x00000004,
        VertexShader = 0x00000008,
        TessellationControlShader = 0x00000010,
        TessellationEvaluationShader = 0x00000020,
        GeometryShader = 0x00000040,
        FragmentShader = 0x00000080,
        EarlyFragmentTests = 0x00000100,
        LateFragmentTests = 0x00000200,
        ColorAttachmentOutput = 0x00000400,
        ComputeShader = 0x00000800,
        Transfer = 0x00001000,
        BottomOfPipe = 0x00002000,
        Host = 0x00004000,
        AllGraphics = 0x00008000,
        AllCommands = 0x00010000,

        None = 0x0,

        MaxEnum = 0x7FFFFFFF // Just here to force the enum to be 32-bits
    };

    using PipelineStages = Flags;
}

#endif // FL_VULKANRHI_ENUMS_HPP
