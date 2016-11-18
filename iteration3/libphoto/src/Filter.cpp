#include "Filter.h"
// For debug lines
#include <iostream>
#include <iomanip>
//---------------

Filter::Filter() {

}
Filter::~Filter() {
}

PositionIndependentFilter::PositionIndependentFilter(void (*fptr)(ColorData*, float*)) {
    innerFun = fptr;
}
PositionIndependentFilter::~PositionIndependentFilter() {

}

void PositionIndependentFilter::apply(PixelBuffer* buf, float *args) {
    for (int i = 0; i < buf->getWidth(); ++i)
    {
        for (int j = 0; j < buf->getHeight(); ++j)
        {
            ColorData pix = buf->getPixel(i, j);
            innerFun(&pix, args);
            buf->setPixel(i, j, pix.clampedColor());
        }
    }
}



ConvolutionBasedFilter::ConvolutionBasedFilter(double** (*fptr)(int&, float*)) {
    filterGen = fptr;
}
ConvolutionBasedFilter::~ConvolutionBasedFilter() {

}

void ConvolutionBasedFilter::apply(PixelBuffer* buf, float *args) {
    // Initialization
    int dspHeight = buf->getHeight();
    int dspWidth = buf->getWidth();
    int filterSize;
    double **filter = filterGen(filterSize, args);
    // Display the filter being use
    // std::cout << "The filter (size=" << filterSize << ") is as follows:" << std::endl;
    // std::cout << std::fixed << std::setprecision(2);
    // for (int i = 0; i < filterSize; ++i)
    // {
    //     for (int j = 0; j < filterSize; ++j)
    //     {
    //         std::cout << filter[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout.unsetf(std::ios_base::floatfield);
    // Use a copy for reading so changes don't affect convolution
    PixelBuffer *image = new PixelBuffer(dspWidth, dspHeight, buf->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(buf, image);
    // Application of filter kernel
    // Code modified from http://lodev.org/cgtutor/filtering.html
    for (int x = 0; x < dspWidth; x++) {
    for (int y = 0; y < dspHeight; y++) {
        ColorData *newPixel = new ColorData(0, 0, 0);
        // Build new pixel using surrounding pixels prescribed in kernel
        for (int filterY = 0; filterY < filterSize; ++filterY) {
        for (int filterX = 0; filterX < filterSize; ++filterX) {
            // Wrap around image edges
            int imageX = (x - filterSize / 2 + filterX + dspWidth) % dspWidth;
            int imageY = (y - filterSize / 2 + filterY + dspHeight) % dspHeight;
            *newPixel = *newPixel + (image->getPixel(imageX, imageY) * filter[filterY][filterX]);
        }
        }
        // Update original buffer
        buf->setPixel(x, y, newPixel->clampedColor());
        delete newPixel;
    }
    }
    for (int i = 0; i < filterSize; i++)
        delete[] filter[i];
    delete[] filter;
}

ImageMaskBasedFilter::ImageMaskBasedFilter(double** (*fptr)(int, int, float*)) {
    imageMaskGen = fptr;
}
ImageMaskBasedFilter::~ImageMaskBasedFilter() {

}

void ImageMaskBasedFilter::apply(PixelBuffer* buf, float *args) {
    int width, height;
    width = buf->getWidth();
    height = buf->getHeight();
    double **mask = imageMaskGen(width, height, args);
    // Apply the mask
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            buf->setPixel(i, j, buf->getPixel(i, j) * mask[i][j]);
        }
    }


}