#pragma once

#include <stdint.h>
#include "types.h"

#if defined(__GNUC__)
    #define INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define INLINE __forceinline
#else
    #define INLINE
#endif

#define API
#define PRIVATE static

#define PIXEL_INDEX(self, x, y)\
    (self->pitch * (y)) + ((x) * self->channels)

#include "types.h"
#include "error.h"
#include "colorspace.h"
#include "image.h"
#include "io/png.h"
