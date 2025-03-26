// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Win32/Win32Utils.hpp>

namespace Fl {
    time_t FileTimeToTime(const FILETIME* time) {
        SYSTEMTIME stUTC, stLocal;

        FileTimeToSystemTime(time, &stUTC);
        SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);

        std::tm timeInfo;
        timeInfo.tm_sec = stLocal.wSecond;
        timeInfo.tm_min = stLocal.wMinute;
        timeInfo.tm_hour = stLocal.wHour;
        timeInfo.tm_mday = stLocal.wDay;
        timeInfo.tm_mon = stLocal.wMonth - 1;
        timeInfo.tm_year = stLocal.wYear - 1900;
        timeInfo.tm_isdst = -1;

        return std::mktime(&timeInfo);
    }
}

#include <FlashlightEngine/Core/AntiWindows.hpp>