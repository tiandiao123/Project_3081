#include <assert.h>
#include <stdlib.h>
#include <cmath>
#include "Mask.h"

MaskOpacitySettings::MaskOpacitySettings() {}

MaskOpacitySettings::MaskOpacitySettings(MaskOpacityType i_type, float a) {
    type = i_type;
    coeff[0] = a;
}

MaskOpacitySettings::MaskOpacitySettings(MaskOpacityType i_type, float a, float b) {
    type = i_type;
    coeff[0] = a;
    coeff[1] = b;
}

MaskOpacitySettings::MaskOpacitySettings(MaskOpacityType i_type, float a, float b, float c) {
    type = i_type;
    coeff[0] = a;
    coeff[1] = b;
    coeff[2] = c;
}

//////////////////////
//       Mask      //
////////////////////

Mask::Mask() {
    m_size = 0;
}
Mask::~Mask() {
    destroy();
}

int Mask::getSize() {
    return m_size;
}

MaskOpacitySettings *Mask::Settings()
{
    return &m_settings;
}

float **Mask::OpacityData() {
    return m_data;
}

// Mask protected
void Mask::setSettings(MaskOpacitySettings settings) {
    m_settings = settings;
}

int Mask::allocate() {
    assert(m_size > 0);
    m_data = new float *[m_size];
    for (int i = 0; i < m_size; i++)
        m_data[i] = new float[m_size];
    return 1;
}

void Mask::build() {
    //memset(m_data, m_size * m_size * sizeof(float), 0);
}

// void Mask::debug() {
//     for (int i = 0; i < m_size; ++i) {
//         for (int j = 0; j < m_size; ++j) {
//             if (m_data[i][j] < 1.0e-6) std::cout << 0 << " ";
//             else std::cout << m_data[i][j] << " ";
//         }
//         std::cout << std::endl;
//     }
// }

float Mask::buildpoint(float dist_cent) {
    float val;
    switch (m_settings.type) {
        case SOLID:
            val = m_settings.coeff[0];
            break;
        case LINEARDECAY:
            // y = mx + b
            val = m_settings.coeff[1] * dist_cent * -1 + m_settings.coeff[0];
            break;
        case EXPONENTIALDECAY:
            // y = ax^2 + bx + c
            val = m_settings.coeff[2] * dist_cent * dist_cent + m_settings.coeff[1] * dist_cent + m_settings.coeff[0];
            break;
        default:
            val = 0.0;
    }
    if (val < 0.0) val = 0.0;
    return val;
}

void Mask::destroy() {
    for (int i = 0; i < m_size; i++)
            if (m_data[i] != NULL) delete [] m_data[i];
    delete [] m_data;
}

void Mask::setSize(int size) {
    if (m_size > 0) destroy();
    m_size = size;
    allocate();
}


//////////////////////
//  CIRCULAR MASK  //
////////////////////

// CircularMask public
CircularMask::CircularMask(int diam, MaskOpacitySettings settings) {
    setDiameter(diam);
    setSettings(settings);
    build();
}
CircularMask::~CircularMask() {}

int CircularMask::getDiameter() {
    return m_diam;
}

// CircularMask protected
void CircularMask::setDiameter(int diam) {
    assert(diam > 0);
    m_diam = diam;
    setSize(m_diam);
}

void CircularMask::build() {
    // Find the center of the mask
    int center = m_diam/2;
    for (int i = 0; i < m_diam; ++i) {
        for (int j = 0; j < m_diam; ++j) {
            float val, dist;
            // Calculate distance from center
            dist = sqrt((i-center)*(i-center)+(j-center)*(j-center));
            if (dist > m_diam/2.0)
                val = 0.0;
            else
                val = buildpoint(dist);
            m_data[i][j] = val;
        }
    }
}

///////////////////////
// RECTANGULAR MASK //
/////////////////////

// RectangularMask public
RectangularMask::RectangularMask(int width, int height, MaskOpacitySettings settings) {
    m_width = width;
    m_height = height;
    updateSize();
    setSettings(settings);
    build();
}
RectangularMask::~RectangularMask() {}

int RectangularMask::getWidth() {
    return m_width;
}
int RectangularMask::getHeight() {
    return m_height;
}

// RectangularMask protected
void RectangularMask::setWidth(int width) {
    m_width = width;
    updateSize();
}
void RectangularMask::setHeight(int height) {
    m_height = height;
    updateSize();
}
void RectangularMask::build() {
    // Need to shift so that it is centered (aka don't start at 0)
    int s_width, s_height = 0;
    if (m_width < getSize()) s_width = (getSize() - m_width)/2;
    else s_height = (getSize() - m_height)/2;
    for (int i = s_width; i < m_width + s_width; ++i) {
        for (int j = s_height; j < m_height + m_height; ++j) {
            // Build item at [i,j]
            float dist = 0.0; // Do we need decay in rectangles?
            m_data[i][j] = buildpoint(dist);

        }
    }
}

// RectangularMask private
void RectangularMask::updateSize() {
    if (m_width > m_height) setSize(m_width);
    else setSize(m_height);
}