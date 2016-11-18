

enum MaskOpacityType
{
    SOLID,
    LINEARDECAY,
    EXPONENTIALDECAY
};

class MaskOpacitySettings
{
  public:
    MaskOpacityType type;
    float coeff[10];
    MaskOpacitySettings();
    MaskOpacitySettings(MaskOpacityType i_type, float a);
    MaskOpacitySettings(MaskOpacityType i_type, float a, float b);
    MaskOpacitySettings(MaskOpacityType i_type, float a, float b, float c);
};

class Mask
{
public:
    Mask();
    ~Mask();
    int getSize();
    MaskOpacitySettings *Settings();
    float **OpacityData();
    void debug();

protected:
    int allocate(); // Allocates memory for mask
    virtual void build();
    float buildpoint(float dist_cent);
    void destroy(); // Frees memory for mask
    void setSize(int size);
    void setSettings(MaskOpacitySettings settings);
    float **m_data;
    MaskOpacitySettings m_settings;

private:
    int m_size;

};

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


