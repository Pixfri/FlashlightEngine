// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_RENDERER_ENUMS_HPP
#define FL_RENDERER_ENUMS_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    enum class RendererValidationLevel {
        None = 0,

        Verbose,
        Info,
        Warning,
        Error
    };

}

#endif // FL_RENDERER_ENUMS_HPP
