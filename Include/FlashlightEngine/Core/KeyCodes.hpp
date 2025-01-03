// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef FL_CORE_KEYCODES_HPP
#define FL_CORE_KEYCODES_HPP

#include <FlashlightEngine/Types.hpp>

namespace FlashlightEngine {
    namespace Key {
        enum Key : UInt32 {
            // Printable keys
            Space = 32,
            Apostrophe = 39,
            Comma = 44,
            Minus = 45,
            Period = 46,
            Slash = 47,
            N0 = 48,
            N1 = 49,
            N2 = 50,
            N3 = 51,
            N4 = 52,
            N5 = 53,
            N6 = 54,
            N7 = 55,
            N8 = 56,
            N9 = 57,
            Semicolon = 59,
            Equal = 61,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LBracket = 91,
            BackSlash = 92,
            RBracket = 93,
            GraveAccent = 96,

            World1 = 161,
            World2 = 162,

            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,

            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,

            PageUp = 266,
            PageDown = 267,

            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 282,
            Pause = 283,

            Kp0 = 320,
            Kp1 = 321,
            Kp2 = 322,
            Kp3 = 323,
            Kp4 = 324,
            Kp5 = 325,
            Kp6 = 326,
            Kp7 = 327,
            Kp8 = 328,
            Kp9 = 329,
            KpDecimal = 330,
            KpDivide = 331,
            KpMultiply = 332,
            KpSubtract = 333,
            KpAdd = 334,
            KpEnter = 335,
            KpEqual = 336,

            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,

            LeftShift = 340,
            LeftCtrl = 341,
            LeftAlt = 342,
            LeftSuper = 343,

            RightShift = 344,
            RightCtrl = 345,
            RightAlt = 346,
            RightSuper = 347,

            Menu = 348
        };
    }

    namespace Mouse {
        enum Mouse : UInt32 {
            Left = 0,
            Right = 1,
            Middle = 2,

            Button4 = 3,
            Button5 = 4,
            Button6 = 5,
            Button7 = 6,
            Button8 = 7,
        };
    }
}

#endif // FL_CORE_KEYCODES_HPP
