// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/FunctionRef.hpp>

#include <catch2/catch_test_macros.hpp>

namespace FL_ANONYMOUS_NAMESPACE {
    void foo(int& value) { value = 42; }

    TEST_CASE("[Core/FunctionRef] Has Value") {
        Fl::FunctionRef<void()> func;
        CHECK(!func.operator bool());
        auto lambda = [](){};
        func = lambda;
        CHECK(func.operator bool());
    }

    TEST_CASE("[Core/FunctionRef] Lambda") {
        int value = 0;
        auto lambda = [&value]() { value = 42; };
        const Fl::FunctionRef<void()> func = lambda;
        func();
        CHECK(value == 42);
    }

    TEST_CASE("[Core/FunctionRef] Function") {
        const Fl::FunctionRef<void(int&)> func = foo;
        int value = 0;
        func(value);
        CHECK(value == 42);
    }

    TEST_CASE("[Core/FunctionRef] Args") {
        auto lambda = [](std::size_t& value, const std::string& str) { value = str.size(); };
        Fl::FunctionRef<void(std::size_t&, const std::string&)> func = lambda;
        std::size_t value = 0;
        std::string str = "Hello";
        func(value, str);
        CHECK(value == str.size());
    }
}