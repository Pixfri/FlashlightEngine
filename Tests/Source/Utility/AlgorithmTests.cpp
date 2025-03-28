// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Utility/Algorithm.hpp>

#include <catch2/catch_test_macros.hpp>

#include <map>
#include <string>

struct A {};
struct B : A {};

SCENARIO("Algorithm", "[Algorithm]") {
    WHEN("Testing IntegerToPointer and PointerToInteger") {
        using IntType = std::conditional_t<sizeof(void*) == 8, Fl::UInt64, Fl::UInt32>;

        A a;
        CHECK(Fl::IntegerToPointer<A*>(Fl::PointerToInteger<IntType>(&a)) == &a);
    }

    WHEN("Testing Retrieve") {
        using namespace std::literals;

        std::map<std::string, std::size_t, std::less<>> strMap = {
            {
                { "Foo", 1 },
                { "Bar", 2 },
                { "Baz", 3 },
            }
        };

        CHECK(Fl::Retrieve(strMap, "Foo") == 1);
        CHECK(Fl::Retrieve(strMap, "Bar") == 2);
        CHECK(Fl::Retrieve(strMap, "Baz") == 3);

        CHECK(Fl::Retrieve(strMap, "Foo"sv) == 1);
        CHECK(Fl::Retrieve(strMap, "Bar"sv) == 2);
        CHECK(Fl::Retrieve(strMap, "Baz"sv) == 3);

        CHECK(Fl::Retrieve(strMap, "Foo"s) == 1);
        CHECK(Fl::Retrieve(strMap, "Bar"s) == 2);
        CHECK(Fl::Retrieve(strMap, "Baz"s) == 3);
    }

    WHEN("Testing SafeCast") {
#ifdef FL_HAS_CONSTEVAL
        static_assert(Fl::SafeCast<Fl::UInt64>(3.0) == 3);
        static_assert(Fl::SafeCast<Fl::UInt64>(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
        static_assert(Fl::SafeCast<int>(static_cast<Fl::UInt64>(42)) == 42);
#endif


        // Catch2 offers no way to catch an assertion failure
        CHECK(Fl::SafeCast<Fl::UInt64>(3.0) == 3);
        CHECK(Fl::SafeCast<Fl::UInt64>(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
        CHECK(Fl::SafeCast<int>(Fl::UInt64(42)) == 42);
        int v = Fl::SafeCaster(static_cast<Fl::UInt64>(42));
        CHECK(v == 42);

        B b;
        A* a = &b;

        CHECK(Fl::SafeCast<B*>(a) == &b);
        CHECK(Fl::SafeCast<B*>(nullptr) == nullptr);
    }
}