#include "FilterHandler.h"

FilterHandler::FilterHandler() {
    for (int i = 0; i < FILTER_COUNT; ++i)
    {
        switch (i) {
            case BLUR:
                filters[i] = dynamic_cast<Filter*> (new ConvolutionBasedFilter(&KernelFilterAlgorithm::kernelBlur));
                break;
            case MOTIONBLUR:
                filters[i] = dynamic_cast<Filter*> (new ConvolutionBasedFilter(&KernelFilterAlgorithm::kernelMotionBlur));
                break;
            case QUANTIZE:
                filters[i] = dynamic_cast<Filter*> (new PositionIndependentFilter(&PixelFilterAlgorithm::quantizePixel));
                break;
            case THRESHOLD:
                filters[i] = dynamic_cast<Filter*> (new PositionIndependentFilter(&PixelFilterAlgorithm::adjustPixelToThreshold));
                break;
            case SATURATION:
                filters[i] = dynamic_cast<Filter*> (new PositionIndependentFilter(&PixelFilterAlgorithm::adjustPixelSaturation));
                break;
            case RGB:
                filters[i] = dynamic_cast<Filter*> (new PositionIndependentFilter(&PixelFilterAlgorithm::adjustPixelRGB));
                break;
            case EDGEDETECTION:
                filters[i] = dynamic_cast<Filter*> (new ConvolutionBasedFilter(&KernelFilterAlgorithm::kernelEdgeDetection));
                break;
            case SHARPEN:
                filters[i] = dynamic_cast<Filter*> (new ConvolutionBasedFilter(&KernelFilterAlgorithm::kernelSharpen));
                break;
            case SEPIA:
                filters[i] = dynamic_cast<Filter*> (new PositionIndependentFilter(&PixelFilterAlgorithm::sepiaPixel));
                break;
           case VIGNETTE:
               filters[i] = dynamic_cast<Filter*> (new ImageMaskBasedFilter(&ImageMaskAlgorithm::vignetteMask));
               break;
        }
    }
}

FilterHandler::~FilterHandler() {
    for (int i = 0; i < FILTER_COUNT; ++i)
        delete filters[i];
}

Filter* FilterHandler::getFilter(int i) {
    return filters[i];
}

void FilterHandler::apply(PixelBuffer *buf, int f, float *args) {
    filters[f]->apply(buf, args);
}
