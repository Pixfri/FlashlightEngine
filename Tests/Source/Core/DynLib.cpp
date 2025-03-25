// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/DynLib.hpp>

#include <catch2/catch_test_macros.hpp>

#ifdef FL_PLATFORM_POSIX
#define PREFIX "lib"
#else
#define PREFIX ""
#endif

namespace FL_PLATFORM_WINDOWS {
    TEST_CASE("[Core/DynLib] Invoke") {
        Fl::DynLib lib;
        bool result = lib.Load(PREFIX "flashlightTest-dummy");
        REQUIRE(result);

        lib.Invoke<void>("Dummy");
        const Fl::FunctionRef func = lib.GetFunction<void>("Dummy");
        CHECK(func.operator bool() == true);
        {
            const int value = lib.Invoke<int>("DummyInt");
            CHECK(value == 42);
        }
        {
            const int* globalValue = lib.GetValue<int>("GlobalInt");
            if (globalValue == nullptr) {
                FAIL();
            }
            CHECK(globalValue != nullptr);
            CHECK(*globalValue == 42);
        }
        {
            const int* notExisting = lib.GetValue<int>("NotExisting");
            CHECK(notExisting == nullptr);
        }
        {
            const int value = lib.Invoke<int>("Increment", 5);
            CHECK(value == 6);
        }
        {
            CHECK_THROWS(lib.Invoke<int>("NotExisting", 5));
        }

        result = lib.Unload();
        CHECK(result == true);
    }

    TEST_CASE("[Core/DynLib] Loading") {
        Fl::DynLib lib;
        CHECK_FALSE(lib.Unload());
        bool result = lib.Load(PREFIX "not-existing");
        CHECK_FALSE(result);
        CHECK(lib.GetSymbol("foo") == nullptr);

        result = lib.Load(PREFIX "flashlightTest-dummy");
        CHECK(result == true);

        result = lib.Unload();
        CHECK(result == true);
    }
} // namespace FL_PLATFORM_WINDOWS

