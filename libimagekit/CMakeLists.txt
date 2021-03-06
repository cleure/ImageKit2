cmake_minimum_required (VERSION 2.6)
project (libimagekit)

# Turn DEBUG on
add_definitions(-DDEBUG)

set (libimagekit_VERSION_MAJOR 2)
set (libimagekit_VERSION_MINOR 0)
set (libimagekit_VERSION_PATCH 0)
set (CMAKE_C_FLAGS "-Wall -std=c99 -g")

set(LINK_LIBRARIES)
set(SOURCE_FILES
    # ImageKit Sources
    image.c
    colorspace.c
    io/png.c
    io/jpeg.c
)

#add_subdirectory(tests)
include_directories(${PROJECT_SOURCE_DIR})
#include_directories(htable)

# JPEG
find_package (JPEG)
if (JPEG_FOUND)
    include_directories(${JPEG_INCLUDE_DIRS})
    LIST(APPEND LINK_LIBRARIES ${JPEG_LIBRARIES})
    add_definitions(-DHAVE_JPEG)
endif (JPEG_FOUND)

# PNG
find_package (PNG)
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    LIST(APPEND LINK_LIBRARIES ${PNG_LIBRARIES})
    add_definitions(-DHAVE_PNG)
endif (PNG_FOUND)

# Threads
find_package (Threads)
LIST(APPEND LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

# Math
LIST(APPEND LINK_LIBRARIES m)

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

add_library(imagekit ${SOURCE_FILES})

add_executable(test test.c)
target_link_libraries(test imagekit ${LINK_LIBRARIES})

#
#add_executable(tests/build/test-01-png tests/test-01-png.c tests/framework.c)
#target_link_libraries(tests/build/test-01-png imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-02-jpeg tests/test-02-jpeg.c tests/framework.c)
#target_link_libraries(tests/build/test-02-jpeg imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-03-hsv tests/test-03-hsv.c tests/framework.c)
#target_link_libraries(tests/build/test-03-hsv imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-04-mono tests/test-04-mono.c tests/framework.c)
#target_link_libraries(tests/build/test-04-mono imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-05-jpeg tests/test-05-jpeg.c tests/framework.c)
#target_link_libraries(tests/build/test-05-jpeg imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-06-fill tests/test-06-fill.c tests/framework.c)
#target_link_libraries(tests/build/test-06-fill imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-07-histogram tests/test-07-histogram.c tests/framework.c)
#target_link_libraries(tests/build/test-07-histogram imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-08-blit tests/test-08-blit.c tests/framework.c)
#target_link_libraries(tests/build/test-08-blit imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-09-bezier tests/test-09-bezier.c tests/framework.c)
#target_link_libraries(tests/build/test-09-bezier imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-10-pointfilter tests/test-10-pointfilter.c tests/framework.c)
#target_link_libraries(tests/build/test-10-pointfilter imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-11-errors tests/test-11-errors.c tests/framework.c htable/MurmurHash3.c htable/hashtable.c)
#target_link_libraries(tests/build/test-11-errors imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-12-png30 tests/test-12-png30.c tests/framework.c)
#target_link_libraries(tests/build/test-12-png30 imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-13-rankfilter tests/test-13-rankfilter.c tests/framework.c)
#target_link_libraries(tests/build/test-13-rankfilter imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-14-convolution tests/test-14-convolution.c tests/framework.c)
#target_link_libraries(tests/build/test-14-convolution imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-15-nearest tests/test-15-nearest.c tests/framework.c)
#target_link_libraries(tests/build/test-15-nearest imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-16-bilinear tests/test-16-bilinear.c tests/framework.c)
#target_link_libraries(tests/build/test-16-bilinear imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-17-complex tests/test-17-complex.c tests/framework.c)
#target_link_libraries(tests/build/test-17-complex imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-18-remove-alpha tests/test-18-remove-alpha.c tests/framework.c)
#target_link_libraries(tests/build/test-18-remove-alpha imagekit ${LINK_LIBRARIES})
#
#add_executable(tests/build/test-19-bresenham tests/test-19-bresenham.c tests/framework.c)
#target_link_libraries(tests/build/test-19-bresenham imagekit ${LINK_LIBRARIES})
#
