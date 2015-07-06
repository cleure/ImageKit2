#pragma once

typedef struct ImageKit_Error {
    uint32_t code;
    char msg[64];
} ImageKit_Error;

#define ImageKit_NoError                0
#define ImageKit_NotImplementedError    1
#define ImageKit_StandardError          2
#define ImageKit_ValueError             3
#define ImageKit_TypeError              4
#define ImageKit_RangeError             5
#define ImageKit_OSError                6
#define ImageKit_IOError                7
#define ImageKit_MemoryError            8
#define ImageKit_IndexError             9
#define ImageKit_ArgumentError          10
#define IMAGEKIT_NUM_ERRORS             11
