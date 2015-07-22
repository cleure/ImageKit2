#pragma once

#include <stdint.h>
#include <math.h>
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

#define MAX(r, s) ((r) + (s) + fabs((r) - (s))) / 2
#define MIN(r, s) ((r) + (s) - fabs((r) - (s))) / 2
#define CLAMP(minValue, maxValue, value) MAX(minValue, MIN(maxValue, value))

#include "types.h"
#include "error.h"
#include "colorspace.h"
#include "image.h"
#include "io/png.h"
#include "io/jpeg.h"
