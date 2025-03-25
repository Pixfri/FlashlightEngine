// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Prerequisites.hpp>

#if defined(FL_PLATFORM_WINDOWS)
#   include <Windows.h>
namespace Fl {
    bool IsDebuggerAttached() {
        return IsDebuggerPresent() != 0;
    }
}
#elif defined(FL_PLATFORM_POSIX)
#include <fstream>

namespace Fl {
    bool IsDebuggerAttached() {
        std::ifstream statusFile("/proc/self/status");
        std::string line;
        while (std::getline(statusFile, line)) {
            if (line.substr(0, 11) == "TracerPid:\t") {
                int tracerPid = std::stoi(line.substr(11));
                return tracerPid != 0;
            }
        }

        return false;
    }
}
#else
namespace Fl {
    bool IsDebuggerAttached() {
        return false;
    }
}
#endif
