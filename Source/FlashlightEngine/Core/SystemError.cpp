// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/SystemError.hpp>

#if defined(FL_PLATFORM_WINDOWS)
#   include <Windows.h>
#elif defined(FL_PLATFORM_POSIX)
#   include <cstring>
#   include <errno.h>
#else
#   error Unsupported platform.
#endif

namespace Fl {
    std::string SystemError::GetLastSystemError() {
#if defined(FL_PLATFORM_WINDOWS)
        const DWORD error = GetLastError();
#   if defined(FL_DEBUG)
        const auto hResult = HRESULT_FROM_WIN32(error);
        [[maybe_unused]] const auto facility = HRESULT_FACILITY(hResult);
        [[maybe_unused]] const auto severity = HRESULT_SEVERITY(hResult);
        [[maybe_unused]] const auto code = HRESULT_CODE(hResult);
#   endif

        LPSTR messageBuffer = nullptr;

        const UInt64 size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM     |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&messageBuffer),
            0,
            nullptr
        );

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);

        return message;
#elif defined(FL_PLATFORM_POSIX)
        return std::strerror(errno);
#else
#   error No implementation was provided for this platform.
        return "Not supported.";
#endif
    }
}
