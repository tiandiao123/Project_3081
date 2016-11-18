#include "PixelBuffer.h"
#include "ColorData.h"

#ifndef FILTER_H
#define FILTER_H

/// A wrapper class for image filters
/** At their simplest, image filters apply an algorith to an entire pixel buffer */
class Filter
{
public:
    Filter();
    virtual ~Filter();

    /// Applies the filter to a pixel buffer
    virtual void apply(PixelBuffer* buf, float *args) = 0;

};

/// A Filter implementation that does not require knowledge of where each modified pixel is located in the image
/** PositionIndependentFilters apply a simple function to each ColorData (pixel) object of a PixelBuffer independently.
    Inner functions included in the library are located in PixelFilterAlgorithm */
class PositionIndependentFilter : public Filter
{
public:
    PositionIndependentFilter(void (*fptr)(ColorData*, float*)); ///< Constructor requires a pointer to the function to apply to each pixel given args
    ~PositionIndependentFilter();

    void (*innerFun)(ColorData*, float*); ///< Internal storage of the function pointer

    void apply(PixelBuffer* buf, float *args); ///< Uses nested loops to iterate over the PixelBuffer, applying the innerFunction at each pixel ColorData
};

/// A Filter implementation that convolves the value of each pixel.
/** Convolution uses a mask of values centered on the current pixel being edited.
    The values correspond to the influence of surrounding pixels on the new value of the center pixel.
    Functions for convolution mask generation included in the library are located in KernelFilterAlgorithm */
class ConvolutionBasedFilter : public Filter
{
public:
    ConvolutionBasedFilter(double** (*fptr)(int&, float*)); ///< Constructor requires a pointer to the function that generates a convolution mask given a variable for size and args
    ~ConvolutionBasedFilter();

    double** (*filterGen)(int&, float*); ///< Internal storage of the function pointer

    /// Applies the convolution filter by:
    /// - Generating a filter from the filterGen function, returning an array of doubles and a size (byref)
    /// - Creates a copy of the PixelBuffer for reading unchanged pixel values
    /// - Iterates over every ColorData in the buffer
    /// - Iterates over the mask size using surrounding ColorData values to adjust the current Pixel
    /// - Cleans up the filter array
    void apply(PixelBuffer* buf, float *args);
};

/// A Filter implementation that applies a mask to an entire image
/** ImageMaskBasedFilters use a set of values from 0->1 corresponding to the amount a ColorData pixel should be multiplied by. */
class ImageMaskBasedFilter : public Filter
{
public:
    ImageMaskBasedFilter(double** (*fptr)(int, int, float*)); ///< Constructor requires a pointer to the function that generates an image mask given the size of the image and args
    ~ImageMaskBasedFilter();

    double** (*imageMaskGen)(int, int, float*); ///< Internal storage of the function pointer

    void apply(PixelBuffer* buf, float *args); ///< Applies the filter by generating the mask then iterating through each pixel multiplying it by the corresponding mask value
};

#endif