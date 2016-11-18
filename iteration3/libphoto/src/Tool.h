#include "Mask.h"
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Filter.h"

/// Color mixing method enumberation used to specify the type of color blending
enum ColorMixingMethod {
    BASICBLEND,
    PHOTONFLUXBLEND,
    LUMINANCEBLEND,
};

/// A parent/wrapper class for tools
/** Holds a virtual function for applying, which handles the details of applying each tool to the canvas.
 Contains helper functions used for applying.
 Further specified into 4 subclasses: MaskBasedTool, SampleBasedTool, ImageBasedTool, and FilterBasedTool.
*/
class Tool
{
public:
    Tool();
    virtual ~Tool();

    virtual void apply(int x, int y, PixelBuffer* buf, ColorData fg) = 0; ///< Applies the tool to the PixelBuffer passed in as the argument at the specified x,y coordinates
    void fillGap(int new_x, int new_y, PixelBuffer* buf, ColorData fg); ///< Helper function used to fill gaps between application events (makes tool application look smoother)
    void setToolApplying(); ///< Switches the state of m_tool_applying, a flag used to determine if fillGap() needs to be called
    int getBlendMode(); ///< Returns the blend mode (from the ColorMixingMethod enum) that is stored in m_blendmode
    void setBlendMode(int mode); ///< Sets the value of m_blendmode to the blend mode (from the ColorMixingMethod enum) specified

protected:
    int checkBounds(int x, int y, PixelBuffer* buf); ///< Checks bounds to see if x,y are legal coordinates for buf
    ColorData blendColor(ColorData cur_col, ColorData new_col, float opac_new, int mixing_method); ///< Helper function to build a color from the the current color and new fg color applied
    float photonfluxblendColorVal(float a, float b, float opac); ///< Helper function used when the photon flux blend mode is specified
    int m_blendmode;
    int m_size; ///< Member variable that stores the size of the tool
    int m_tool_applying; ///< Indicates if the tool is currently being applied, used to determine if fillGap() will be called
    int m_drag_prev_x; ///< x coordinate from previous application (used by fillGap())
    int m_drag_prev_y;///< y coordinate from previous application (used by fillGap())
};

/** A tool that contains a Mask object that is applied to the canvas */
class MaskBasedTool : public Tool
{
public:
    MaskBasedTool();
    MaskBasedTool(Mask* mask, int blend_mode); ///< This constructor takes as input a pointer to a Mask object and the blend mode used by the tool
    ~MaskBasedTool();

    Mask* getMask(); ///< Returns a pointer to the Mask object contained in the tool
    void setMask(Mask* mask); ///< Sets the tool's mask to be identical to the Mask specified
    void apply(int x, int y, PixelBuffer* buf, ColorData fg); ///< Applies the mask to the PixelBuffer specified

protected:
    Mask* m_mask;
};

/** A tool that takes a sample from another portion of the PixelBuffer and copies it to the point at which the tool is applied */
class SampleBasedTool : public Tool
{
public:
    SampleBasedTool();
    SampleBasedTool(int size, int x_off, int y_off, float blend_per, int blend_mode); ///< This constructor takes as input the size of the sample area, the x,y offset, and the blend settings
    ~SampleBasedTool();
    void apply(int x, int y, PixelBuffer* buf, ColorData fg); ///< Applies the sample to the specified x,y coordinates of the PixelBuffer
    int m_xoffset;
    int m_yoffset;
    float m_blend_opac;
};

/** A tool that stores an image and applies that image at the specified location */
class ImageBasedTool : public Tool
{
public:
	ImageBasedTool();
    ImageBasedTool(PixelBuffer *buf); ///< This constructor takes as argument the image to be applied
    ~ImageBasedTool();

	void apply(int x, int y, PixelBuffer* buf, ColorData fg); ///< Applies the image to the specified PixelBuffer
	void updateStamp(PixelBuffer *buf); ///< Updates the image that is stored by the tool

private:
	PixelBuffer *stamp;
};

/** A tool that contains an image filter, which will be applied to an area of a specified size */
class FilterBasedTool : public MaskBasedTool
{
public:
    FilterBasedTool(Mask* mask, int mode, Filter* filter, float *filter_args, int argsize); ///< This constructor takes as argument the filter, along with its paramaters
    ~FilterBasedTool();

    void apply(int x, int y, PixelBuffer* buf, ColorData fg); ///< Applies the filter to specified portion of the PixelBuffer

private:
    Filter *m_filter; ///< Stores the filter used by the tool
    float *m_filterargs; ///< Stores the arguments that will be passed into the filter
    static const int EXTRA_SPACING = 50;
};
