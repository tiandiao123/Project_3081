//
//  PixelBuffer.h
//  Originally created by the CSci-3081W TAs.
//

#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

class ColorData;

/** The PixelBuffer class stores an array of ColorData, such as an image that
    can be drawn to the screen. */
class PixelBuffer
{
public:

    PixelBuffer(int w, int h, ColorData backgroundColor);
    virtual ~PixelBuffer();

    /// Sets the color of a specific pixel
    void setPixel(int x, int y, const ColorData& color);

    /// Fills the enitre pixel buffer with the specified color
    void fillPixelBufferWithColor(ColorData color);


    /// Returns the color of a specific pixel
    ColorData getPixel(int x, int y) const;

    /// Returns a pointer to the raw ColorData array for fast access to ColorData
    ColorData const * const getData() const;

    /// Returns the background color that was used to initialize the PixelBuffer
    ColorData getBackgroundColor();

    int getHeight() const;
    int getWidth() const;

    /// A static method to copy one pixel buffer to another
    static void copyPixelBuffer(PixelBuffer * sourceBuffer, PixelBuffer * destinationBuffer);

private:

    /// Array of pixel colors
    ColorData * m_pixels;

    /// Pointer to the single color used as the "background color" to initialize the PixelBuffer
    ColorData * m_backgroundColor;

    // Dimensions
    const int m_width;
    const int m_height;
};

#endif
