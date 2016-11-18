/// Opacity type enumeration for MaskOpacitySettings
enum MaskOpacityType
{
    SOLID,
    LINEARDECAY,
    EXPONENTIALDECAY
};

/// Settings to be passed into Mask for automatic building of values
class MaskOpacitySettings
{
  public:
    MaskOpacitySettings();
    MaskOpacitySettings(MaskOpacityType i_type, float a); ///< Constructor for SOLID type. Takes a value to set the entire mask to: p = a
    MaskOpacitySettings(MaskOpacityType i_type, float a, float b); ///< Constructor for LINEARDECAY type. Uses coefficients in format: p = -b*(dist_center) + a
    MaskOpacitySettings(MaskOpacityType i_type, float a, float b, float c); ///< Constructor for EXPONENTIALDECAY type. Uses coefficients in format: p = c*(dist_center**2) + b*(dist_center) + a

    MaskOpacityType type;
    float coeff[10]; ///< Holds coefficient values passed into the settings
};

/// A parent/wrapper class for masks
/** Holds a 2D array of floating point values from 0..1.
 Values often represent opacity of the item being passed through the mask. 0 means none gets through, 1 means it all does.
 Handles memory allocation and destruction and dynamic sizing of array.
 Simplifies mask operations by always assuming a square */
class Mask
{
public:
    Mask();
    ~Mask();
    int getSize(); ///< Returns the current size of the mask as the length of a single dimension (since square)
    MaskOpacitySettings *Settings(); ///< Returns a pointer to the current settings object
    float **OpacityData(); ///< Allows access to the float array
    void debug(); ///< Pretty prints the values of the mask array

protected:
    int allocate(); ///< Allocates memory for mask
    virtual void build(); ///< To be implemented by child classes, typically constructs a shaped mask by calling build point
    float buildpoint(float dist_cent); ///< Calculates a value for a point in the mask using the coefficients and type given by MaskOpacitySettings
    void destroy(); ///< Frees memory for mask
    void setSize(int size); ///< Updates the size of the mask
    void setSettings(MaskOpacitySettings settings); ///< Updates the MaskOpacitySettings of the mask
    float **m_data; ///< Pointer to the 2D float array
    MaskOpacitySettings m_settings;

private:
    int m_size;

};

/// A circular implementation of Mask
/** Creates a circular mask by setting all values outside the diameter of the circle to 0 */
class CircularMask : public Mask
{
public:
    CircularMask(int diam, MaskOpacitySettings settings);
    ~CircularMask();

    int getDiameter();

protected:
    void setDiameter(int diam);
    void build();

private:
    int m_diam;
};

/// A rectangular implementation of Mask.
/** Creates a rectangular mask by creating a square to the larger side and centering a rectangle in it, setting values outside the rectangle to 0 */
class RectangularMask : public Mask
{
public:
    RectangularMask(int width, int height, MaskOpacitySettings settings);
    ~RectangularMask();

    int getWidth();
    int getHeight();

protected:
    void build();
    void setWidth(int width);
    void setHeight(int height);

private:
    int m_width;
    int m_height;
    void updateSize();
};


