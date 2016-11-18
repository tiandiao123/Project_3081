#include "PixelBuffer.h"
#include "ColorData.h"

#ifndef FILTER_H
#define FILTER_H

class Filter
{
public:
    Filter();
    virtual ~Filter();

    // Applies the filter to a pixel buffer
    virtual void apply(PixelBuffer* buf, float *args) = 0;

};

class PositionIndependentFilter : public Filter
{
public:
    PositionIndependentFilter(void (*fptr)(ColorData*, float*));
    ~PositionIndependentFilter();

    void (*innerFun)(ColorData*, float*);

    void apply(PixelBuffer* buf, float *args);
};

class ConvolutionBasedFilter : public Filter
{
public:
    ConvolutionBasedFilter(double** (*fptr)(int&, float*));
    ~ConvolutionBasedFilter();

    double** (*filterGen)(int&, float*);

    void apply(PixelBuffer* buf, float *args);
};

class ImageMaskBasedFilter : public Filter
{
public:
    ImageMaskBasedFilter(double** (*fptr)(int, int, float*));
    ~ImageMaskBasedFilter();

    double** (*imageMaskGen)(int, int, float*);

    void apply(PixelBuffer* buf, float *args);
};

#endif