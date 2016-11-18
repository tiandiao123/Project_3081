#include "ColorData.h"

// Due to a weird function undefined compilation error
// the static functions could not be moved to a CPP file
// :( I tried to figure it out for a long while then gave
// up
namespace PixelFilterAlgorithm
{
    // All algorithms below take a single pixel as ColorData
    // and adjusts it byref as needed

    static void adjustPixelToThreshold(ColorData* pixel, float *args) {
        float t = args[0];

        if (pixel->getGreen() < t)
            pixel->setGreen(0);
        else
            pixel->setGreen(1);

        if (pixel->getRed() < t)
            pixel->setRed(0);
        else
            pixel->setRed(1);

        if (pixel->getBlue() < t)
            pixel->setBlue(0);
        else
            pixel->setBlue(1);
    }

    static void adjustPixelSaturation(ColorData* pixel, float *args) {
        float value = args[0];
        float luminance = pixel->getLuminance();
        pixel->setGreen(pixel->getGreen()*value + (1.0-value)*luminance);
        pixel->setRed(pixel->getRed()*value + (1.0-value)*luminance);
        pixel->setBlue(pixel->getBlue()*value + (1.0-value)*luminance);
    }

    static void adjustPixelRGB(ColorData *pixel, float *args) {
        float rf = args[0];
        float gf = args[1];
        float bf = args[2];
        pixel->setGreen(pixel->getGreen() * gf);
        pixel->setRed(pixel->getRed() * rf);
        pixel->setBlue(pixel->getBlue() * bf);
    }

    static void quantizePixel(ColorData *pixel, float *args) {
        int size = int(args[0]);

        float* binsvalue= new float[size];

        for (int i=0; i < size; i++)
            binsvalue[i] = i / float(size - 1);

        float colorarray[4]= {pixel->getGreen(), pixel->getRed(), pixel->getBlue(), pixel->getAlpha()};
        float intensity[4];

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < size; i++) {
                if (colorarray[j] >= i/float(size) && colorarray[j] <= (i+1)/float(size))
                    intensity[j] = i/float((size - 1));
            }
        }

        pixel->setGreen(intensity[0]);
        pixel->setRed(intensity[1]);
        pixel->setBlue(intensity[2]);
        pixel->setAlpha(intensity[3]);
    }

    static void sepiaPixel(ColorData *pixel, float *args) {
        pixel->setRed((pixel->getRed() * .393) + (pixel->getGreen() *.769) + (pixel->getBlue() * .189));
        pixel->setGreen((pixel->getRed() * .349) + (pixel->getGreen() *.686) + (pixel->getBlue() * .168));
        pixel->setBlue((pixel->getRed() * .272) + (pixel->getGreen() *.534) + (pixel->getBlue() * .131));
    }
};
