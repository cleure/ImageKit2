#pragma once

#include "types.h"

#define IMAGEKIT_COLORSPACE_RGB     1
#define IMAGEKIT_COLORSPACE_GREY    2
#define IMAGEKIT_COLORSPACE_HSV     3
#define IMAGEKIT_COLORSPACE_HSL     4
#define IMAGEKIT_COLORSPACE_XYZ     5
#define IMAGEKIT_COLORSPACE_LAB     6
#define IMAGEKIT_COLORSPACE_YCBCR   7

API
int
ImageKit_Colorspace_RGB2HSV(
    REAL r,
    REAL g,
    REAL b,
    REAL *oh,
    REAL *os,
    REAL *ov
);

//API
//int
//ImageKit_Colorspace_HSV2RGB(
//    REAL *src,
//    REAL *dst,
//    DIMENSION channels
//);
