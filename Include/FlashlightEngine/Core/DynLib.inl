// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/DynLib.hpp>

#include <FlashlightEngine/Core/SystemError.hpp>

#include <memory>

#if defined(FL_PLATFORM_WINDOWS)
#   include <FlashlightEngine/Core/Win32/DynLibImpl.hpp>
#elif defined(FL_PLATFORM_POSIX)
#   include <FlashlightEngine/Core/Posix/DynLibImpl.hpp>
#else
#   error No implementation for this platform.
#endif

namespace Fl {
}
