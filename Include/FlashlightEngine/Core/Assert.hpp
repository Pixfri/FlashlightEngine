// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_ASSERT_HPP
#define FL_CORE_ASSERT_HPP

#include <FlashlightEngine/Prerequisites.hpp>

#include <spdlog/spdlog.h>

#include <source_location>

namespace Fl {
    template <typename T>
    constexpr void FlAssert(T expr, const std::string& error, const std::source_location& location = std::source_location::current());
}

#include <FlashlightEngine/Core/Assert.inl>

#endif // FL_CORE_ASSERT_HPP
