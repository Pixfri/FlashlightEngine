// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/BaseObject.hpp>

#include <type_traits>

namespace Fl {
    template <class T>
    UInt64 BaseObject::GetId() noexcept {
        static_assert(std::is_base_of_v<BaseObject, T>,
                      "[Error] The class passed to BaseObject::GetId must be derived from Fl::BaseObject.");
        static_assert(!std::is_same_v<BaseObject, T>,
                      "[Error] The class passed to BaseObject::GetId must not be of type Fl::BaseObject.");

        static const UInt64 id = s_maxId++;

        return id;
    }
} // namespace Fl
