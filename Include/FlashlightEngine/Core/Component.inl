// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

namespace Fl {
    template <typename ComponentType>
    U64 Component::GetId() {
        static_assert(std::is_base_of_v<Component, ComponentType>,
                      "[Error] The fetched id must be of a class derived from Fl::Component");
        static_assert(!std::is_same_v<Component, ComponentType>,
                      "[Error] You have to use Component::GetId only on a derived class and not on Fl::Component.");

        static const U64 id = s_MaxId++;
        return id;
    }
}
