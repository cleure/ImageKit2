
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include "imagekit.h"

#ifdef HAVE_PNG

#include <png.h>

/*

Use __LINE__ and __FILE__ for debugging

*/

typedef struct PNG_IO {
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;
    png_uint_16p *row_pointers16;
    
    DIMENSION width;
    DIMENSION height;
    DIMENSION channels;
    DIMENSION depth;
} PNG_IO;

PRIVATE
int
PNG_Open_Read(const char *filepath, PNG_IO *png)
{
    FILE *fp;
    
    fp = fopen(filepath, "rb");
    if (!fp) {
        #ifdef DEBUG
          fprintf(stderr, "error opening file: %s\n", filepath);
        #endif
        
        return -1;
    }
    
    // Read struct
    png->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png->png_ptr) {
        fclose(fp);
        
        #ifdef DEBUG
          fprintf(stderr, "libpng error, could not create read struct\n");
        #endif
        
        return -1;
    }
    
    // Info struct
    png->info_ptr = png_create_info_struct(png->png_ptr);
    if (!png->info_ptr) {
        png_destroy_read_struct(&png->png_ptr, NULL, NULL);
        fclose(fp);
        
        #ifdef DEBUG
          fprintf(stderr, "libpng error, could not create info struct\n");
        #endif
        
        return -1;
    }
    
    // Error handling
    if (setjmp(png_jmpbuf(png->png_ptr))) {
        png_destroy_read_struct(&png->png_ptr, &png->info_ptr, NULL);
        if (fp) {
            fclose(fp);
        }
        
        #ifdef DEBUG
          fprintf(stderr, "libpng error, returned to exception handler\n");
        #endif
        
        return -1;
    }
    
    // Init PNG stuff
    png_init_io(png->png_ptr, fp);
    png_read_png(png->png_ptr, png->info_ptr, PNG_TRANSFORM_IDENTITY | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ALPHA, NULL);
    fclose(fp);
    fp = NULL;
    
    // Get width / height, channels
    png->width = png_get_image_width(png->png_ptr, png->info_ptr);
    png->height = png_get_image_height(png->png_ptr, png->info_ptr);
    png->channels = png_get_channels(png->png_ptr, png->info_ptr);
    png->depth = png_get_bit_depth(png->png_ptr, png->info_ptr);
    
    // Link row pointers
    png->row_pointers = png_get_rows(png->png_ptr, png->info_ptr);
    png->row_pointers16 = (png_uint_16p *)png->row_pointers;
    
    return 0;
}

PRIVATE void PNG_Close(PNG_IO *png)
{
    png_destroy_read_struct(&png->png_ptr, &png->info_ptr, NULL);
    memset(png, 0, sizeof(*png));
}

/*

TODO: Ability to set colorspace on load?

*/

API
ImageKit_Image *
ImageKit_Image_FromPNG(const char *filepath) {
    ImageKit_Image *self;
    PNG_IO png;
    size_t x, y, c;
    REAL scale;
    REAL *ptr;
    int colorspace;
    
    if (PNG_Open_Read(filepath, &png) < 0) {
        return NULL;
    }
    
    if (png.channels < 3) {
        colorspace = IMAGEKIT_COLORSPACE_GREY;
    } else {
        colorspace = IMAGEKIT_COLORSPACE_RGB;
    }
    
    self = ImageKit_Image_New(png.width, png.height, png.channels, colorspace);
    if (!self) {
        PNG_Close(&png);
        
        #ifdef DEBUG
          fprintf(stderr, "error creating image\n");
        #endif
        
        return NULL;
    }
    
    // depth=8, depth=16
    if (png.depth == 8) {
        scale = 1.0 / 255.0;
    } else if (png.depth == 16) {
        scale = 1.0 / 65535.0;
    } else {
        #ifdef DEBUG
          fprintf(stderr, "unknown / unsupported bit-depth %u\n", png.depth);
        #endif
        
        PNG_Close(&png);
        return NULL;
    }
    
    ptr = self->data1;
    
    if (png.depth == 16) {
        for (y = 0; y < self->height; y++) {
            for (x = 0; x < self->width; x++) {
                for (c = 0; c < png.channels; c++) {
                    *ptr++ = (REAL)png.row_pointers16[y][x * png.channels + c] * scale;
                }
            }
        }
    } else {
        for (y = 0; y < self->height; y++) {
            for (x = 0; x < self->width; x++) {
                for (c = 0; c < png.channels; c++) {
                    *ptr++ = (REAL)png.row_pointers[y][x * png.channels + c] * scale;
                }
            }
        }
    }
    
    PNG_Close(&png);
    return self;
}

API
int
ImageKit_Image_SavePNG(ImageKit_Image *self, const char *filepath, uint32_t depth)
{
    FILE *fp;
    REAL *ptr_in;
    REAL scale;
    REAL value;
    int32_t value32;
    size_t x, y, c;
    PNG_IO png;
    png_byte *row = NULL;
    
    if (depth == 0) {
        depth = 8;
    }

    if (depth == 8) {
        scale = 255.0;
    } else if (depth == 16) {
        scale = 65535.0;
    } else {
        #ifdef DEBUG
            fprintf(stderr, "depth must be one of: 8, 10, 16");
        #endif
        
        return -1;
    }
    
    fp = fopen(filepath, "wb");
    if (!fp) {
        #ifdef DEBUG
            fprintf(stderr, "error opening file: %s\n", filepath);
        #endif
        
        return -1;
    }
    
    // Initialize
    png.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png.png_ptr == NULL) {
        fclose(fp);
        #ifdef DEBUG
            fprintf(stderr, "libpng error, error creating write struct\n");
        #endif
        
        return -1;
    }
    
    png.info_ptr = png_create_info_struct(png.png_ptr);
    if (png.info_ptr == NULL) {
        png_destroy_write_struct(&png.png_ptr, NULL);
        fclose(fp);
        #ifdef DEBUG
            fprintf(stderr, "libpng error, error creating info struct\n");
        #endif
        
        return -1;
    }

    // Error Handling
    if (setjmp(png_jmpbuf(png.png_ptr))) {
        png_destroy_write_struct(&png.png_ptr, &png.info_ptr);
        fclose(fp);
        #ifdef DEBUG
            fprintf(stderr, "libpng error, returned to exception handler\n");
        #endif
        
        return -1;
    }
    
    png_set_IHDR(   png.png_ptr,
                    png.info_ptr,
                    self->width,
                    self->height,
                    depth,
                    PNG_COLOR_TYPE_RGB,
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_DEFAULT,
                    PNG_FILTER_TYPE_DEFAULT);
    
    ptr_in = (REAL *)(&(self->data1[0]));
    
    if (depth == 16) {
        // 16 Bits Per Channels
        png.row_pointers = png_malloc(png.png_ptr, sizeof(png_byte *) * self->height);
    
        // Scale, clamp, copy
        for (y = 0; y < self->height; y++) {
            png.row_pointers[y] = png_malloc(png.png_ptr, sizeof(png_byte) * self->width * self->channels * 2);
            row = png.row_pointers[y];
            
            for (x = 0; x < self->width; x++) {
                for (c = 0; c < self->channels; c++) {
                    value = *ptr_in++;
                    value32 = (int32_t)(CLAMP(0.0, 1.0, value) * scale);
                    *row++ = value32 >> 8 & 0xff;
                    *row++ = value32      & 0xff;
                }
            }
        }
    
        // Write file
        png_init_io(png.png_ptr, fp);
        png_set_rows(png.png_ptr, png.info_ptr, png.row_pointers);
        png_write_png(png.png_ptr, png.info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
        for (y = 0; y < self->height; y++) {
            png_free(png.png_ptr, png.row_pointers[y]);
        }
    
        png_free(png.png_ptr, png.row_pointers);
    } else {
        // 8 Bits Per Channels
        png.row_pointers = png_malloc(png.png_ptr, sizeof(png_byte *) * self->height);
    
        // Scale, clamp, copy
        for (y = 0; y < self->height; y++) {
            png.row_pointers[y] = png_malloc(png.png_ptr, sizeof(png_byte) * self->width * self->channels);
            row = png.row_pointers[y];
            
            for (x = 0; x < self->width; x++) {
                for (c = 0; c < self->channels; c++) {
                    value = *ptr_in++;
                    *row++ = (int32_t)(CLAMP(0.0, 1.0, value) * scale);
                }
            }
        }
    
        // Write file
        png_init_io(png.png_ptr, fp);
        png_set_rows(png.png_ptr, png.info_ptr, png.row_pointers);
        png_write_png(png.png_ptr, png.info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
        for (y = 0; y < self->height; y++) {
            png_free(png.png_ptr, png.row_pointers[y]);
        }
    
        png_free(png.png_ptr, png.row_pointers);
    }
    
    png_destroy_write_struct(&png.png_ptr, &png.info_ptr);
    fclose(fp);
    
    return 0;
}

#else

API
ImageKit_Image *
ImageKit_Image_FromPNG(const char *filepath) {
    return NULL;
}

API
int
ImageKit_Image_SavePNG(ImageKit_Image *self, const char *filepath, uint32_t depth) {
    return -1;
}

#endif
