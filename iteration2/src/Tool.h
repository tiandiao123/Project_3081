#include "Mask.h"
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Filter.h"

enum ColorMixingMethod {
    BASICBLEND,
    PHOTONFLUXBLEND,
    LUMINANCEBLEND,
};

class Tool
{
public:
    Tool();
    virtual ~Tool();

    // Applies the tool to a PixelBuffer
    virtual void apply(int x, int y, PixelBuffer* buf, ColorData fg) = 0;
    // Fills gaps between application events
    void fillGap(int new_x, int new_y, PixelBuffer* buf, ColorData fg);
    void setToolApplying();
    // Fills gaps
    int getBlendMode();
    void setBlendMode(int mode);

protected:
    // Checks bounds to see if x and y are legal
    int checkBounds(int x, int y, PixelBuffer* buf);
    // Helper function to build a color from the the current color and new fg color applied
    ColorData blendColor(ColorData cur_col, ColorData new_col, float opac_new, int mixing_method);
    float photonfluxblendColorVal(float a, float b, float opac);
    int m_blendmode;
    int m_size;
    // Indicates if tool is being applied (used to fill gaps)
    int m_tool_applying;
    // x,y coordinates from previous application (used to fill gaps)
    int m_drag_prev_x;
    int m_drag_prev_y;
};

class MaskBasedTool : public Tool
{
public:
    MaskBasedTool();
    MaskBasedTool(Mask* mask, int blend_mode);
    ~MaskBasedTool();

    Mask* getMask();
    void setMask(Mask* mask);
    void apply(int x, int y, PixelBuffer* buf, ColorData fg);

protected:
    Mask* m_mask;
};

class SampleBasedTool : public Tool
{
public:
    SampleBasedTool();
    SampleBasedTool(int size, int x_off, int y_off, float blend_per, int blend_mode);
    ~SampleBasedTool();
    void apply(int x, int y, PixelBuffer* buf, ColorData fg);
    int m_xoffset;
    int m_yoffset;
    float m_blend_opac;
};

class ImageBasedTool : public Tool
{
public:
	ImageBasedTool();
    ImageBasedTool(PixelBuffer *buf);
    ~ImageBasedTool();

	void apply(int x, int y, PixelBuffer* buf, ColorData fg);
	void updateStamp(PixelBuffer *buf);

private:
	PixelBuffer *stamp;
};


class FilterBasedTool : public MaskBasedTool
{
public:
    FilterBasedTool(Mask* mask, int mode, Filter* filter, float *filter_args, int argsize);
    ~FilterBasedTool();

    void apply(int x, int y, PixelBuffer* buf, ColorData fg);

private:
    Filter *m_filter;
    float *m_filterargs;
    static const int EXTRA_SPACING = 50;
};
