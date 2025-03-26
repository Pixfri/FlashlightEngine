// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/DynLib.hpp>

#include <catch2/catch_test_macros.hpp>

#include <functional>

#ifdef FL_PLATFORM_POSIX
#   define PREFIX "lib"
#else
#   define PREFIX ""
#endif

using DummyIntType = int(*)();
using IncrementType = int(*)(int);
using GlobalIntType = int;

SCENARIO("DynLib", "[DynLib]") {
    WHEN("Testing Load/Unload") {
        Fl::DynLib dynLib;

        CHECK_FALSE(dynLib.IsLoaded());

        const bool result = dynLib.Load(PREFIX "flashlightTest-dummy");
        CHECK(result);
        CHECK(dynLib.IsLoaded());

        dynLib.Unload();

        CHECK_FALSE(dynLib.IsLoaded());
    }

    WHEN("Testing GetSymbol") {
        Fl::DynLib dynLib;
        dynLib.Load(PREFIX "flashlightTest-dummy");

        std::function dummy = dynLib.GetSymbol("Dummy");
        CHECK(dummy != nullptr);

        dummy();

        auto dummyInt = reinterpret_cast<DummyIntType>(dynLib.GetSymbol("DummyInt"));
        CHECK(dummyInt != nullptr);
        CHECK(dummyInt() == 42);

        auto increment = reinterpret_cast<IncrementType>(dynLib.GetSymbol("Increment"));
        CHECK(increment != nullptr);
        CHECK(increment(42) == 43);
    }
}
