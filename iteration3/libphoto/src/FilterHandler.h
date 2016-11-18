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

/// An enum of currently supported filter types.
/// Used by FilterHandler to identify filter indices.
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

/// Handles filter class initialization and use
class FilterHandler
{
public:
    FilterHandler(); ///< Initializes the filter array filling each index corresponding to FilterType with the proper Filter subclass and function
    ~FilterHandler();


    Filter* getFilter(int i); ///< Returns a filter class corresponding to the given index or FilterType
    void apply(PixelBuffer *buf, int type, float *args); ///< Passes application to the proper filter class

private:
    Filter *filters[FILTER_COUNT]; ///< Internal storage of Filter instances
};
