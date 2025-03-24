// Copyright (C) 2025 Jean "Pixfri" Letessier
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <FlashlightEngine/Core/BaseObject.hpp>

#include <type_traits>

#if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC)
#   include <cxxabi.h>
#   include <cstdlib>
#   include <memory>
#endif

namespace Fl {
    template <class T>
    BaseObject::ClassInfo BaseObject::GetInfo() noexcept {
        static_assert(std::is_base_of_v<BaseObject, T>,
                      "[Error] The class passed to BaseObject::GetId must be derived from Fl::BaseObject.");
        static_assert(!std::is_same_v<BaseObject, T>,
                      "[Error] The class passed to BaseObject::GetId must not be of type Fl::BaseObject.");

        static const UInt64 id = s_maxId++;

        const auto tempName = typeid(T).name();

#if defined(FL_COMPILER_MSVC)
        // Doesn't seem to need to be demangled with MSVC, but need to remove the class/struct qualifier from the type name.
        auto name = std::string(tempName);
        name = name.substr(name.find_first_of(" \t\f\n\r\v")+1); //< All whitespaces to be sure.

        return {std::move(name), id};
#elif defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC)
        // Needs to be demangled with GCC and clang, so we need to use compiler specific APIs.
        return {DemangleClassName(tempName), id};
#else
#   pragma message No way to retrieve the class name was implemented for this compiler.
#endif
    }

#if defined(FL_COMPILER_CLANG) || defined(FL_COMPILER_GCC)
    // From https://stackoverflow.com/a/1024655
    inline std::string BaseObject::DemangleClassName(const char* mangled) {
        const auto ptr = std::unique_ptr<char, decltype(&std::free)>{
            abi::__cxa_demangle(mangled, nullptr, nullptr, nullptr),
            std::free
        };
        return {ptr.get()};
    }
#endif
} // namespace Fl
