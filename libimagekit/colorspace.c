
#include <math.h>
#include <stdint.h>

#include "imagekit.h"

/*

TODO: What does a greyscale image look like?

[1.0, 0.0, 0.0, 1.0]
[1.0, 1.0]

*/

API
int
ImageKit_Colorspace_RGB2HSV(
    REAL r,
    REAL g,
    REAL b,
    REAL a,
    REAL *oh,
    REAL *os,
    REAL *ov,
    REAL *oa
)
{
    REAL hue;
    REAL saturation;
    REAL value;
    REAL M;
    REAL m;
    REAL C;
    
    // Max
    M = r;
    if (M < g) M = g;
    if (M < b) M = b;
    
    // Min
    m = r;
    if (m > g) m = g;
    if (m > b) m = b;
    
    // Difference ("chroma")
    C = M - m;
    
    value = M;
    saturation = ((M > 0) ? C / M : 0.0);
    
    if (C == 0) {
        hue = 0.0;
    } else if (M == r) {
        // ((g - b) / C) % 6
        hue = fmod((g - b) / C, 6.0);
    } else if (M == g) {
        hue = (b - r) / C + 2.0;
    } else {
        hue = (r - g) / C + 4.0;
    }
    
    *oh = hue / 6.0;
    *os = saturation;
    *ov = value;
    *oa = a;
    
    return 0;
}

API
int
ImageKit_Colorspace_HSV2RGB(
    REAL *src,
    REAL *dst,
    DIMENSION channels
)
{
    return 0;
}
