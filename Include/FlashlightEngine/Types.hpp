// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_TYPES_HPP
#define FL_TYPES_HPP

#include <cstdint>
#include <cstddef>

static_assert(sizeof(int8_t) == 1, "int8_t must be 1 byte long");
static_assert(sizeof(int16_t) == 2, "int16_t must be 2 bytes long");
static_assert(sizeof(int32_t) == 4, "int32_t must be 4 bytes long");
static_assert(sizeof(int64_t) == 8, "int64_t must be 8 bytes long");

static_assert(sizeof(uint8_t) == 1, "uint8_t must be 1 byte long");
static_assert(sizeof(uint16_t) == 2, "uint16_t must be 2 bytes long");
static_assert(sizeof(uint32_t) == 4, "uint32_t must be 4 bytes long");
static_assert(sizeof(uint64_t) == 8, "uint64_t must be 8 bytes long");

static_assert(sizeof(float) == 4, "float must be 4 bytes long");
static_assert(sizeof(double) == 8, "double must be 8 bytes long");

namespace FlashlightEngine {
    using Int8 = int8_t;
    using UInt8 = uint8_t;

    using Int16 = int16_t;
    using UInt16 = uint16_t;

    using Int32 = int32_t;
    using UInt32 = uint32_t;

    using Int64 = int64_t;
    using UInt64 = uint64_t;

    using Float = float;
    using Double = double;

    using Flags = UInt32;
}

#define FlUnused(x) (void)(x)

#endif // FL_TYPES_HPP
