#include "Filter.h"
#include "KernelFilterAlgorithm.h"
#include "PixelFilterAlgorithm.h"
#include "ImageMaskAlgorithm.h"

#ifndef FILTER_COUNT
#define FILTER_COUNT 10
#endif

#ifndef FILTER_MAX_ARGS
#define FILTER_MAX_ARGS 3
#endif

enum FilterType {
    BLUR,
    MOTIONBLUR,
    QUANTIZE,
    THRESHOLD,
    SATURATION,
    RGB,
    EDGEDETECTION,
    SHARPEN,
    SEPIA,
    VIGNETTE
};

class FilterHandler
{
public:
    FilterHandler();
    ~FilterHandler();

    Filter* getFilter(int i);
    void apply(PixelBuffer *buf, int type, float *args);

private:
    Filter *filters[FILTER_COUNT];
};
