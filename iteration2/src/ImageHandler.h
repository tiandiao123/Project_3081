#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <iostream>
#include "PixelBuffer.h"
#include "ColorData.h"
#include "jpeglib.h"
#include "png.h"


class ImageHandler {
public:
    ImageHandler();
    ~ImageHandler();

    int loadImage(std::string fileName, PixelBuffer *&buf);
    int saveImage(std::string fileName, PixelBuffer *&buf);


private:
    /* Helper function for determining file type */
    bool hasSuffix(const std::string & str, const std::string & suffix);

    /* Variables used for both JPEG and PNG files */
    FILE * infile;      // Source file
    FILE * outfile;     // Target file
    int rowStride;      // Physical row width in image buffer

    /* Data used for reading/writing JPEG files */
    struct jpeg_compress_struct cinfo;
    struct jpeg_decompress_struct dinfo;
    struct jpeg_error_mgr cjerr;
    struct jpeg_error_mgr djerr;
    JSAMPLE * imageBuffer;      // Stores entire image buffer
    JSAMPROW rowPointer[1];     // Pointer to an individual JSAMPLE row
    JSAMPARRAY jpegBuffer;      // Stores image buffer when decompressing

    /* Data used for reading/writing PNG files */
    png_image image;
    png_bytep pngBuffer;
};
