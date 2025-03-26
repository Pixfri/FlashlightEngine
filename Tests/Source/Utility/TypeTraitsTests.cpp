// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utility/TypeTraits.hpp>

// This test is done at compile time

int foo(double, float);

static_assert(std::is_same_v<Fl::FunctionPtr<int(double, float)>, decltype(&foo)>);