// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_WRAPPER_INSTANCE_HPP
#define FL_RENDERER_WRAPPER_INSTANCE_HPP

#include <FlashlightEngine/Types.hpp>

#include <FlashlightEngine/Renderer/Enums.hpp>

#include <volk.h>

#include <vector>

namespace FlashlightEngine {
    class Instance {
    public:
        explicit Instance(RendererValidationLevel validationLevel);
        ~Instance();

        Instance(const Instance&) = delete;
        Instance(Instance&& other) noexcept;

        [[nodiscard]] inline VkInstance GetInstance() const;
        [[nodiscard]] inline bool IsValid() const;
        [[nodiscard]] inline VkResult GetLastResult() const;

        Instance& operator=(const Instance&) = delete;
        Instance& operator=(Instance&& other) noexcept;
    
    private:
        VkInstance m_Instance{VK_NULL_HANDLE};

        mutable VkResult m_LastResult{VK_SUCCESS};

        void CreateInstance(RendererValidationLevel validationLevel);

        // Utility functions
        static std::vector<const char*> GetRequiredExtensions();
        static bool CheckExtensionsSupport(const std::vector<const char*>& requiredExtensions);
    };
}

#include <FlashlightEngine/Renderer/Wrapper/Instance.inl>

#endif // FL_RENDERER_WRAPPER_INSTANCE_HPP
