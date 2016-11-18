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

#ifndef IMAGEHAND_H
#define IMAGEHAND_H

/** The ImageHandler class hides all of the implementation details of loading and saving images from the rest of the code.
 To load an image using ImageHanlder, you simply call loadImage() with the appropriate image filename and the PixelBuffer to load the image to.
 To save an image, you call saveImage() with the filename you'd like to save the image to and the PixelBuffer to retrieve the image from.
 This class supports saving and loading of JPEG images and PNG images with transparency.
*/
class ImageHandler {
public:
    ImageHandler();
    ~ImageHandler();

    int loadImage(std::string fileName, PixelBuffer *&buf); ///< loadImage will take the image specified by fileName and load it into buf
    int loadImage(std::string fileName, PixelBuffer *&buf, ColorData bg);
    int saveImage(std::string fileName, PixelBuffer *&buf); ///< saveImage will take the image specified by buf and save it to the file specified by fileName


private:
    bool hasSuffix(const std::string & str, const std::string & suffix); ///< Helper function used to determine the image file type

    /** Variables used for both JPEG and PNG files */
    FILE * infile;      ///< Source file
    FILE * outfile;     //< Target file
    int rowStride;      ///< Physical row width in image buffer

    /** Data used for reading/writing JPEG files */
    struct jpeg_compress_struct cinfo;
    struct jpeg_decompress_struct dinfo;
    struct jpeg_error_mgr cjerr;
    struct jpeg_error_mgr djerr;
    JSAMPLE * imageBuffer;      ///< Stores entire image buffer of a JPEG image
    JSAMPROW rowPointer[1];     ///< Pointer to an individual JSAMPLE row for JPEG images
    JSAMPARRAY jpegBuffer;      ///< Stores image buffer when decompressing a JPEG

    /** Data used for reading/writing PNG files */
    png_image image; ///< Struct used to load/save PNG images
    png_bytep pngBuffer; ///< Buffer used when loading/saving PNG images
};

#endif
