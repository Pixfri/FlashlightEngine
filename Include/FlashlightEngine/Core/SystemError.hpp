// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_SYSTEMERROR_HPP
#define FL_CORE_SYSTEMERROR_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <string>

namespace Fl {
    class FL_API SystemError {
    public:
        static std::string GetLastSystemError();
    };
}

#endif // FL_CORE_SYSTEMERROR_HPP
