// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_INPUT_HPP
#define FL_CORE_INPUT_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    class Input {
    public:
        static bool IsKeyPressed(Int32 keyCode);
        static bool IsMouseButtonPressed(Int32 button);
        static void GetMousePos(Float64& x, Float64& y);
        static Float64 GetMouseX();
        static Float64 GetMouseY();
    };
}

#endif // FL_CORE_INPUT_HPP
