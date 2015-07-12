
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "imagekit.h"

/*
typedef struct ImageKit_ThreadData {
    ImageKit_Image *self;
    DIMENSION x1;
    DIMENSION y1;
    DIMENSION x2;
    DIMENSION y2;
    void (*fn)(REAL *src, REAL *dst, DIMENSION);
} ImageKit_ThreadData;

void randpixel(REAL *src, REAL *dst, DIMENSION channels)
{
    int i;
    for (i = 0; i < channels; i++) {
        dst[i] = src[i] * 0.78;
    }
}

void *
ImageKit_Image_Pointfiler_ApplyFn_Thread(
    ImageKit_ThreadData *threadData
)
{
    size_t index;
    DIMENSION y;
    DIMENSION x;
    DIMENSION x1 = threadData->x1;
    DIMENSION y1 = threadData->y1;
    DIMENSION x2 = threadData->x2;
    DIMENSION y2 = threadData->y2;
    ImageKit_Image *self = threadData->self;
    void (*fn)(REAL *src, REAL *dst, DIMENSION) = threadData->fn;
    
    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            index = (y * self->pitch) + (x * self->channels);
            fn(&self->data1[index], &self->data2[index], self->channels);
        }
    }
    
    ImageKit_Image_SwapBuffers(self);
    
    return NULL;
}

API
int
ImageKit_Image_Pointfiler_ApplyFn(
    ImageKit_Image *self,
    void (*fn)(REAL *src, REAL *dst, DIMENSION)
)
{
    int i;
    int divError;
    int nthreads = 4;
    ImageKit_ThreadData threadData[nthreads];
    pthread_t threads[nthreads];
    
    if (0) {
        threadData[0].self = self;
        threadData[0].fn = fn;
        threadData[0].x1 = 0;
        threadData[0].y1 = 0;
        threadData[0].x2 = self->width;
        threadData[0].y2 = self->height;
        ImageKit_Image_Pointfiler_ApplyFn_Thread(&threadData[0]);
    } else {
        for (i = 0; i < nthreads; i++) {
            threadData[i].self = self;
            threadData[i].fn = fn;

            divError = ImageKit_Image_GetSubDivideRect(
                self,
                &threadData[i].x1,
                &threadData[i].y1,
                &threadData[i].x2,
                &threadData[i].y2,
                4,
                i
            );
            
            if (divError < 0) {
                fprintf(stderr, "TODO: Handle error from ImageKit_Image_GetSubDivideRect()");
                return -1;
            }
            
            printf("%d %d %d %d\n",
                threadData[i].x1,
                threadData[i].y1,
                threadData[i].x2,
                threadData[i].y2
            );
            
            pthread_create(
                &threads[i],
                NULL,
                (void *(*)(void *))ImageKit_Image_Pointfiler_ApplyFn_Thread,
                &threadData[0]
            );
        }
        
        for (i = 0; i < nthreads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    
    ImageKit_Image_SwapBuffers(self);
    
    return 0;
}

// @TODO: Internal, not an API method
API
int
ImageKit_Image_GetSubDivideRect(
    ImageKit_Image *self,
    DIMENSION *x1,
    DIMENSION *y1,
    DIMENSION *x2,
    DIMENSION *y2,
    DIMENSION n,
    DIMENSION i
)
{
    double rect_width = (double)self->width / (double)n;
    
    if (self->width < 32 || self->height < 32) {
        ImageKit_Image_SetError(self, ImageKit_RangeError, "width/height must be at least 32px");
        return -1;
    }
    
    *x1 = (DIMENSION)(floor(rect_width) * i);
    *x2 = (DIMENSION)(MIN(self->width, ((i + 1 < n) ? *x1 : self->width) + rect_width));
    *y1 = 0;
    *y2 = self->height;

    return 0;
}
*/

/*

max(r, s) = (r + s + |r - s|)/2
min(r, s) = (r + s - |r - s|)/2

*/

int main(int argc, char **argv)
{
    ImageKit_Image *im;
    DIMENSION x, y;
    
    srand(0xfffe);
    
    im = ImageKit_Image_FromPNG("./images/colors.png");
    if (im == NULL) {
        exit(1);
    }
    
    ImageKit_Image_SavePNG(im, "output.png", 8);
    
    printf("%f %f %f\n", im->data1[0], im->data1[1], im->data1[2]);

    exit(0);
}
