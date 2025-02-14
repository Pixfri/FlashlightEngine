// Copyright (C) 2025 Jean "Pixfri" Letessier 
// This file is part of FlashlightEngine.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <FlashlightEngine/Core/Profiler.hpp>

#ifdef TRACY_ENABLE

#include <cstdlib>

void* operator new(size_t count) {
	auto ptr = malloc(count);
	TracyAlloc(ptr, count);

	return ptr;
}

void operator delete(void* ptr) noexcept {
	TracyFree(ptr);
	free(ptr);
}

#endif