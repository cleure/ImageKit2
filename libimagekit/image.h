#pragma once

/*

TODO:
    - get pixel
    - get rect
    - set pixel
    - set rect

*/

API typedef struct ImageKit_Image {
    ImageKit_Error error;
    
    int colorspace;
    DIMENSION width;
    DIMENSION height;
    DIMENSION channels;
    DIMENSION pitch;
    
    REAL *data1;
    REAL *data2;
} ImageKit_Image;

API
ImageKit_Image *
ImageKit_Image_New(
    DIMENSION width,
    DIMENSION height,
    DIMENSION channels,
    int colorspace
);

API
void
ImageKit_Image_Delete(
    ImageKit_Image *self
);

API
ImageKit_Image *
ImageKit_Image_Clone(
    ImageKit_Image *self
);

API
void
ImageKit_Image_SetError(
    ImageKit_Image *self,
    uint32_t code,
    char *msg
);

API
void
ImageKit_Image_SetPixel(
    ImageKit_Image *self,
    DIMENSION x,
    DIMENSION y,
    REAL *data
);

void
ImageKit_Image_SwapBuffers(
    ImageKit_Image *self
);
