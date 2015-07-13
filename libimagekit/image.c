
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "imagekit.h"

API
ImageKit_Image *
ImageKit_Image_New(
    DIMENSION width,
    DIMENSION height,
    DIMENSION channels
)
{
    ImageKit_Image *self;
    REAL *data1;
    REAL *data2;
    size_t data_items = width * height * channels;
    size_t data_size = sizeof(*data1) * data_items;
    
    self = malloc(sizeof(*self));
    if (!self) {
        return NULL;
    }
    
    data1 = malloc(data_size);
    if (!data1) {
        free(self);
        return NULL;
    }
    
    data2 = malloc(data_size);
    if (!data2) {
        free(data1);
        free(self);
        return NULL;
    }
    
    memset(data1, 0, data_size);
    memset(data2, 0, data_size);
    memset(&self->error, 0, sizeof(self->error));
    
    self->width = width;
    self->height = height;
    self->channels = channels;
    self->pitch = width * channels;
    self->data1 = data1;
    self->data2 = data2;
    
    return self;
}

API
void
ImageKit_Image_Delete(
    ImageKit_Image *self
)
{
    free(self->data1);
    free(self->data2);
    free(self);
}

API
void
ImageKit_Image_SwapBuffers(
    ImageKit_Image *self
)
{
    REAL *tmp;
    tmp = self->data1;
    self->data1 = self->data2;
    self->data2 = tmp;
}

API
ImageKit_Image *
ImageKit_Image_Clone(
    ImageKit_Image *self
)
{
    ImageKit_Image *clone;
    size_t data_items = self->width * self->height * self->channels;
    size_t data_size = sizeof(*(self->data1)) * data_items;
    
    clone = ImageKit_Image_New(self->width, self->height, self->channels);
    if (!clone) {
        return NULL;
    }
    
    memcpy(clone->data1, self->data1, data_size);
    memcpy(clone->data2, self->data2, data_size);
    
    return clone;
}

API
void
ImageKit_Image_SetPixel(
    ImageKit_Image *self,
    DIMENSION x,
    DIMENSION y,
    REAL *data
)
{
    DIMENSION c;
    REAL *ptr = &self->data1[PIXEL_INDEX(self, x, y)];
    
    for (c = 0; c < self->channels; c++) {
        *ptr++ = *data++;
    }
}

API
void
ImageKit_Image_SetError(
    ImageKit_Image *self,
    uint32_t code,
    char *msg
)
{
    ImageKit_Error *error = &((*self).error);

    error->code = code;
    strncpy(error->msg, msg, sizeof(error->msg));
}
