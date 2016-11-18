#include "ImageHandler.h"


ImageHandler::ImageHandler() {
    /* Initialize JPEG compression object */
    cinfo.err = jpeg_std_error(&cjerr);
    jpeg_create_compress(&cinfo);

    /* Initialize JPEG decompression object */
    dinfo.err = jpeg_std_error(&djerr);
    jpeg_create_decompress(&dinfo);
}
ImageHandler::~ImageHandler() {}

/* Function copied from FlashPhotoApp, easier than passing function */
bool ImageHandler::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}

int ImageHandler::loadImage(std::string fileName, PixelBuffer *&buf, ColorData bg) {
    /* This function returns 0 if an error has occurred, 1 otherwise */
    const char *filePath = fileName.c_str();

    /* Check what the file type is */
    if (hasSuffix(fileName, ".png")) {
        /* Initialize png_image structure */
        memset(&image, 0, (sizeof image));
        image.version = PNG_IMAGE_VERSION;

        /* Start reading file */
        if (png_image_begin_read_from_file(&image, filePath) != 0) {
            /* Set image to RGBA format */
            image.format = PNG_FORMAT_RGBA;

            /* Allocate memory to hold image */
            pngBuffer = (png_byte *) malloc(PNG_IMAGE_SIZE(image));

            /* Read the full image */
            if (pngBuffer != NULL && png_image_finish_read(&image, NULL, pngBuffer, 0, NULL) != 0)
            {
                /* Set rowStride and initialize replacement buffer */
                rowStride = ((int) image.width) * 4;
                PixelBuffer *replace = new PixelBuffer((int) image.width, (int) image.height, bg);

                /* Loop through pngBuffer and set pixels in replace */
                for(unsigned int y=0;y<image.height;y++) {
                    for(unsigned int x=0;x<rowStride;x+=4) {
                        ColorData pixel;
                        pixel.setRed(((float) pngBuffer[y*rowStride+x]) / 255.0);
                        pixel.setGreen(((float) pngBuffer[y*rowStride+x+1]) / 255.0);
                        pixel.setBlue(((float) pngBuffer[y*rowStride+x+2]) / 255.0);
                        pixel.setAlpha(((float) pngBuffer[y*rowStride+x+3]) / 255.0);

                        replace->setPixel(x/4,(replace->getHeight())-y-1,pixel);  // Adjusts for inverse y
                    }
                }

                buf = replace;

                return 1;
            }
            else { return 0; }    // Error
        }
        else { return 0; }    // Error
    }
    else if (hasSuffix(fileName, ".jpeg") || hasSuffix(fileName, ".jpg")) {
        /* Open file and set as source for decompression */
        if ((infile = fopen(filePath, "rb")) == NULL) { return 0; }
        jpeg_stdio_src(&dinfo, infile);

        /* Get file parameters and start JPEG decompressor */
        (void) jpeg_read_header(&dinfo, TRUE);
        (void) jpeg_start_decompress(&dinfo);

        /* Set rowStride, allocate space for the jpegBuffer and replacement buffer */
        rowStride = dinfo.output_width * dinfo.output_components;
        jpegBuffer = (*dinfo.mem->alloc_sarray) ((j_common_ptr) &dinfo, JPOOL_IMAGE, rowStride, 1);
        PixelBuffer *replace = new PixelBuffer(dinfo.output_width, dinfo.output_height, bg);

        /* Loop through lines of JPEG and decompress */
        int y=0;    // Used to track the row number
        while (dinfo.output_scanline < dinfo.output_height) {
            /* Get next line */
            (void) jpeg_read_scanlines(&dinfo, jpegBuffer, 1);

            /* Loop through line and convert each pixel to a ColorData, then set the pixel in replace */
            for(unsigned int x=0;x<rowStride;x=x+3) {
                ColorData pixel;
                pixel.setRed(((float) jpegBuffer[0][x]) / 255.0);
                pixel.setGreen(((float) jpegBuffer[0][x+1]) / 255.0);
                pixel.setBlue(((float) jpegBuffer[0][x+2]) / 255.0);
                pixel.setAlpha(1.0);

                replace->setPixel(x/3,(replace->getHeight())-y-1,pixel);    // Adjusts for inverse y
            }
            y++;
        }

        /* Finish decompression and close input file */
        (void) jpeg_finish_decompress(&dinfo);
        fclose(infile);

        buf = replace;
        return 1;
    }
}

int ImageHandler::loadImage(std::string fileName, PixelBuffer *&buf) {
    return loadImage(fileName, buf, buf->getBackgroundColor());
}

int ImageHandler::saveImage(std::string fileName, PixelBuffer *&buf) {
    /* This function returns 0 if an error has occurred, 1 otherwise */
    const char* filePath = fileName.c_str();

    /* Check what the file type is */
    if (hasSuffix(fileName, ".png")) {
        /* Convert from PixelBuffer to png_byte array */
        int buffer_size = buf->getHeight() * buf->getWidth() * 4;
        pngBuffer = new png_byte[buffer_size];

        int i = 0;
        for(int y=0;y<buf->getHeight();y++) {
            for(int x=0;x<buf->getWidth();x++) {
                pngBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getRed()) * 255);
                pngBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getGreen()) * 255);
                pngBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getBlue()) * 255);
                pngBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getAlpha()) * 255);
            }
        }

        /* Set rowStride and Initialize png_image structure */
        rowStride = buf->getWidth() * 4;
        memset(&image, 0, (sizeof image));
          image.version = PNG_IMAGE_VERSION;
        image.opaque = NULL;
        image.width = buf->getWidth();
        image.height = buf->getHeight();
        image.format = PNG_FORMAT_RGBA;
        image.flags = 0;

        /* Write file */
        if (pngBuffer == NULL || png_image_write_to_file(&image, filePath, 0, pngBuffer, rowStride, NULL) == 0) { return 0; }

        /* If no failure, return successfully */
        return 1;
    }
    else if (hasSuffix(fileName, ".jpeg") || hasSuffix(fileName, ".jpg")) {
        /* Convert from PixelBuffer to JSAMPLE array */
        int buffer_size = buf->getHeight() * buf->getWidth() * 3;
        imageBuffer = new JSAMPLE[buffer_size];

        int i = 0;
        for(unsigned int y=0;y<buf->getHeight();y++) {
            for(unsigned int x=0;x<buf->getWidth();x++) {
                imageBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getRed()) * 255);
                imageBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getGreen()) * 255);
                imageBuffer[i++] = (unsigned char) ((buf->getPixel(x,buf->getHeight()-1-y).getBlue()) * 255);
            }
        }

        /* Open file and set as destination for compression */
        if ((outfile = fopen(filePath, "wb")) == NULL) { return 0; }
        jpeg_stdio_dest(&cinfo, outfile);

        /* Set parameters for compression, start JPEG compressor */
        rowStride = buf->getWidth() * 3;
        cinfo.image_width = buf->getWidth();
        cinfo.image_height = buf->getHeight();
        cinfo.input_components = 3;
        cinfo.in_color_space = JCS_RGB;
        jpeg_set_defaults(&cinfo);      // Fill parameters with library defaults
        jpeg_start_compress(&cinfo, TRUE);

        /* Loop through image buffer and compress */
        while (cinfo.next_scanline < cinfo.image_height) {
            rowPointer[0] = &(imageBuffer[cinfo.next_scanline * rowStride]);
            (void) jpeg_write_scanlines(&cinfo, rowPointer, 1);
        }

        /* Finish compression and close output file */
        jpeg_finish_compress(&cinfo);
        fclose(outfile);

        /* Return successfully */
        return 1;
    }
}