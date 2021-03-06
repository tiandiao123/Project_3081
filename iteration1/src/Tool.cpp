#include "Tool.h"
#include <cmath>

/////////////////////////////////////////////
/////               TOOL               /////
///////////////////////////////////////////

Tool::Tool() {}
Tool::~Tool() {}

int Tool::checkBounds(int x, int y, PixelBuffer* buf) {
    if ((x < 0) || (x >= buf->getWidth()) || (y < 0) || (y >= buf->getHeight())) return 0;
    else return 1;
}


/////////////////////
// COLOR BLENDING //
///////////////////

void Tool::setBlendMode(int mode) {
    m_blendmode = mode;
}
int Tool::getBlendMode() {
    return m_blendmode;
}

ColorData Tool::blendColor(ColorData col_cur, ColorData col_new, float opac_new, int mixing_method) {
    // There are several techniques to build new colors
    // and accordingly, I will use several here.
    // Account for error in floats, handle transparent painting quickly
    if (opac_new < 1.0e-6) return col_cur;
    ColorData ret;
    // Select a mixing method and blend to get a new pixel ColorData
    switch (mixing_method) {
        case BASICBLEND:
            col_new = col_new*opac_new;
            col_cur = col_cur*(1-opac_new);
            ret = col_cur + col_new;
            break;
        case PHOTONFLUXBLEND:
            ret.setRed(photonfluxblendColorVal(col_cur.getRed(), col_new.getRed(), opac_new));
            ret.setGreen(photonfluxblendColorVal(col_cur.getGreen(), col_new.getGreen(), opac_new));
            ret.setBlue(photonfluxblendColorVal(col_cur.getBlue(), col_new.getBlue(), opac_new));
            ret.setAlpha(1.0);
            break;
        case LUMINANCEBLEND:
            ret = col_new * opac_new * col_cur.getLuminance()
                + col_cur * (1.0 - opac_new * col_cur.getLuminance());
            break;
        default:
            ret = col_new;
    }
    return ret;
}

float Tool::photonfluxblendColorVal(float a, float b, float opac) {
    return sqrt((1 - opac)*a*a + opac*b*b);
}

///////////////////////////
// LINEAR INTERPOLATION //
/////////////////////////

void Tool::fillGap(int new_x, int new_y, PixelBuffer* buf, ColorData fg) {
	// This function is used to fill gaps between applications of the tool

    // Tool is starting to apply, no gap to fill
    if(!m_tool_applying) {
        // Update previous x,y
        m_drag_prev_x = new_x;
        m_drag_prev_y = new_y;
        return;
    }

	// Distance between new x,y and old x,y
	float x_distance = (float) new_x - (float) m_drag_prev_x;
	float y_distance = (float) new_y - (float) m_drag_prev_y;
	// Distance between new point and old point
	float distance = sqrt((x_distance*x_distance) + (y_distance*y_distance));
	// Multiplier for stepping through the for loop that fills the gap
	float step = 1 / (distance / m_size) / 2;

    // x,y coordinates with which to call apply
    int x_apply, y_apply;
    // Apply at each step
    for(float i=0.0;i<1.0;i+=step) {
        x_apply = floor(i*(x_distance)) + m_drag_prev_x;
        y_apply = floor(i*(y_distance)) + m_drag_prev_y;
        apply(x_apply, y_apply, buf, fg);
    }

    // Set previous x,y to current x,y
    m_drag_prev_x = new_x;
    m_drag_prev_y = new_y;
}

void Tool::setToolApplying() {
    m_tool_applying = !m_tool_applying;
}


//////////////////////
// MASK BASED TOOL //
////////////////////

MaskBasedTool::MaskBasedTool() {}
MaskBasedTool::MaskBasedTool(Mask* mask, int mode) {
    setBlendMode(mode);
    setMask(mask);
    m_tool_applying = 0;
    m_size = mask->getSize();
}
MaskBasedTool::~MaskBasedTool() {}

void MaskBasedTool::setMask(Mask* mask) {
    m_mask = mask;
}
Mask* MaskBasedTool::getMask() {
    return m_mask;
}



void MaskBasedTool::apply(int x, int y, PixelBuffer* buf, ColorData fg) {
    // Grab important mask information
    float **mask = m_mask->OpacityData();
    int size = m_mask->getSize();
    // Find the upper left corner of area to apply to
    int app_x = x - size/2;
    int app_y = y + size/2;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // Find the real coordinates, fixing y inversion
            int x = app_x + i;
            int y = buf->getHeight() - app_y + j;
            // Update pixel if applicable
            if (mask[i][j] > 1.0e-6 && checkBounds(x, y, buf)) {
                ColorData col_new = blendColor(buf->getPixel(x, y), fg, mask[i][j], m_blendmode);
                buf->setPixel(x, y, col_new);
            }
        }
    }
}

////////////////////////
// SAMPLE BASED TOOL //
//////////////////////

SampleBasedTool::SampleBasedTool() {}
SampleBasedTool::~SampleBasedTool() {}
SampleBasedTool::SampleBasedTool(int size, int x_off, int y_off, float blend_percent, int blend_mode) {
    m_size = size;
    m_xoffset = x_off;
    m_yoffset = y_off;
    m_blend_opac = blend_percent;
    m_blendmode = blend_mode;
}


void SampleBasedTool::apply(int x, int y, PixelBuffer* buf, ColorData fg) {
    // Find the top left corner of application and loop
    int app_x = x - (m_size)/2;
    int app_y = y + (m_size)/2;
    for (int i = 0; i < m_size; i++)
    {
        for (int j = 0; j < m_size; j++)
        {
            // Find the real coordinates, fixing y inversion
            int x = app_x + i;
            int y = buf->getHeight() - app_y + j;
            // Sample from the offset
            int x_off, y_off;
            x_off = x + m_xoffset;
            y_off = y + m_yoffset;
            // Update pixel if applicable
            if (checkBounds(x_off, y_off, buf) && checkBounds(x, y, buf)) {
                ColorData col_new = blendColor(buf->getPixel(i, j), buf->getPixel(x_off, y_off), m_blend_opac, PHOTONFLUXBLEND);
                buf->setPixel(x, y, col_new);
            }
        }
    }
}
