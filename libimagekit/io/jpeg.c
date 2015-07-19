
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include "imagekit.h"

#ifdef HAVE_JPEG

#include <jpeglib.h>

struct _jpeg_error_handler {
    struct jpeg_error_mgr err;
    char error_msg[1024];
    jmp_buf jmpbuf;
};

PRIVATE void _jpeg_error_exit(j_common_ptr ptr)
{
    /* Get error pointer */
    struct _jpeg_error_handler *err = (struct _jpeg_error_handler *)(ptr->err);
    
    /* Set error message */
    memset(&(err->error_msg), 0, 1024);
    err->err.format_message(ptr, (char *)&(err->error_msg));
    
    /* Long jump, so error can be handled */
    longjmp(err->jmpbuf, 1);
    return;
}

PRIVATE void _jpeg_emit_message(j_common_ptr ptr, int level) {return;}
PRIVATE void _jpeg_output_message(j_common_ptr ptr) {return;}

// TOOD: Common code between JPEG and PNG
PRIVATE uint32_t MaxNumberForNBits(uint32_t n)
{
    uint64_t max = 0;
    
    while (n-- > 0) {
        max |= 1 << n;
    }
    
    return max;
}

API
ImageKit_Image *
ImageKit_Image_FromJPEG(const char *filepath)
{
    ImageKit_Image *self;
    FILE *fp;
    
    /* libjpeg variables */
    struct _jpeg_error_handler error_handler;
    struct jpeg_decompress_struct cinfo;
    int pitch, res;
    JSAMPARRAY buffer;
    
    REAL *ptr_out;
    REAL input_scale = 1.0;
//    int colorspace = CS(RGB);
//    int colorspace_format = CS_FMT(RGB24);
    //REAL *format;

    size_t x;
    uint32_t width, height, channels, depth;
    //uint32_t depth;
    
    fp = fopen(filepath, "rb");
    if (!fp) {
        #ifdef DEBUG
            fprintf(stderr, "unable to open file %s\n", filepath);
        #endif
        
        return NULL;
    }
    
    /* Setup JPEG Errors */
    cinfo.err = jpeg_std_error(&error_handler.err);
    error_handler.err.error_exit = &_jpeg_error_exit;
    error_handler.err.emit_message = &_jpeg_emit_message;
    error_handler.err.output_message = &_jpeg_output_message;

    /* Exception style error handling */
    if (setjmp(error_handler.jmpbuf)) {
        #ifdef DEBUG
            fprintf(stderr, "%s\n", (char *)(&error_handler.error_msg));
        #endif
    
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return NULL;
    }
    
    /* JPEG Stuff */
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, 1);
    
    /* Set decompress options */
    cinfo.dct_method = JDCT_FLOAT;
    cinfo.dither_mode = JDITHER_NONE;
    cinfo.out_color_space = JCS_RGB;
    
    jpeg_start_decompress(&cinfo);
    
    width = cinfo.output_width;
    height = cinfo.output_height;
    depth = cinfo.data_precision;
    channels = cinfo.output_components;
    input_scale = 1.0 / (double)MaxNumberForNBits(depth);
    
    /* Create image instance */
    self = ImageKit_Image_New(width, height);
    
    if (!self) {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return NULL;
    }

    pitch = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, pitch, 1);

    ptr_out = &(self->data1[0]);
    while (cinfo.output_scanline < cinfo.output_height) {
        res = jpeg_read_scanlines(&cinfo, buffer, 1);
        if (res < 1) {
            #ifdef DEBUG
                fprintf(stderr, "jpeg_read_scanlines() < 1, exiting loop\n");
            #endif
            
            break;
        }
        
        for (x = 0; x < self->pitch; x++) {
            ptr_out[x] = (REAL)(buffer[0][x]) * input_scale;
        }
        
        ptr_out += self->pitch;
    }
    
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return self;
}

API
int
ImageKit_Image_SaveJPEG(ImageKit_Image *self, const char *filepath, uint32_t depth)
{
    return -1;
}

#else

API
ImageKit_Image *
ImageKit_Image_FromJPEG(const char *filepath)
{
    return NULL;
}

API
int
ImageKit_Image_SaveJPEG(ImageKit_Image *self, const char *filepath, uint32_t depth)
{
    return -1;
}

#endif
