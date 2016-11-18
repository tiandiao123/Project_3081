//
//  MIAApp.h
//  Originally created by the CSci-3081W TAs.
//


#ifndef MIAAPP_H
#define MIAAPP_H


#include "BaseGfxApp.h"
#include "Tool.h"
#include "ImageHandler.h"
#include "FilterHandler.h"
#include "PixelBufferHistory.h"

const std::string ARROW_PATH("marker.png");

class ColorData;
class PixelBuffer;


/** MIAApp is the Medical Imaging Program; it inherits from BaseGfxApp. */
class MIAApp : public BaseGfxApp {
public:

    MIAApp(int argc, char* argv[], int width, int height, ColorData backgroundColor);
    virtual ~MIAApp();

    // Glut overrided function
    void mouseDragged(int x, int y) ;
    void mouseMoved(int x, int y);
    void leftMouseDown(int x, int y);
    void leftMouseUp(int x, int y);
    void display();
    void gluiControl(int controlID);

private:

    // GLUI INTERFACE ELEMENTS
    enum UIControlType {
        UI_TOOLTYPE,
        UI_FILE_BROWSER,
        UI_LOAD_CANVAS_BUTTON,
        UI_SAVE_CANVAS_BUTTON,
        UI_NEXT_IMAGE_BUTTON,
        UI_PREVIOUS_IMAGE_BUTTON,
        UI_FILE_NAME,
        UI_APPLY_SHARP,
        UI_APPLY_EDGE,
        UI_APPLY_THRESHOLD,
        UI_APPLY_GRAYSCALE,
        UI_APPLY_MULTIPLY_RGB,
        UI_APPLY_QUANTIZE,
        UI_UNDO,
        UI_REDO,
        UI_QUIT
    };
    void setImageFile(const std::string & filepath);
    bool isValidImageFileName(const std::string & name);
    bool isValidImageFile(const std::string & name);
    bool hasSuffix(const std::string & str, const std::string & suffix);
    std::string getImageNamePlusSeqOffset(const std::string & currentFileName, int offset);
    void buttonEnabled(GLUI_Button * button, bool enabled);
    void undoEnabled(bool enabled);
    void redoEnabled(bool enabled);
    void saveCanvasEnabled(bool enabled);
    void loadCanvasEnabled(bool enabled);
    void previousImageEnabled(bool enabled);
    void nextImageEnabled(bool enabled);

    void loadImageToCanvas();
    void saveCanvasToFile();
    void loadNextImage();
    void loadPreviousImage();
    void storeCanvasState();

    void applyFilterSharpen();
    void applyFilterEdgeDetect();
    void applyFilterThreshold();
    void applyFilterMultiplyRGB();
    void applyFilterGrayScale();
    void applyFilterQuantize();
    void applypixelbuffer(double** filtermask,int length);

    void undoOperation();
    void redoOperation();


    void initGlui();
    void initGraphics();
    void initializeBuffers(ColorData initialColor, int width, int height);
    void initializeImageLibs(); ///< Initializes image libraries for MIA (instantiates an ImageHandler)
    void initializeFilters();   ///< Initializes all of the image filters for MIA
    void initializeTools();     ///< Initializes each of the tools used by MIA


    struct {
        float multiply_colorRed;
        float multiply_colorGreen;
        float multiply_colorBlue;
        float threshold_amount;
        float sharpen_amount;
        int quantize_bins;
    } m_filterParameters;

    struct {
        GLUI_FileBrowser* fileBrowser;
        GLUI_Button *loadCanvasButton;
        GLUI_Button *saveCanvasButton;
        GLUI_Button* nextImageButton;
        GLUI_Button* previousImageButton;
        GLUI_Button *redoButton;
        GLUI_Button *undoButton;

        GLUI_StaticText * currentFileLabel;
        GLUI_EditText* fileNameBox;
        GLUI_StaticText * saveFileLabel;




    } m_gluiControlHooks;


    PixelBuffer* m_displayBuffer; ///< This is the pointer to the buffer where the display PixelBuffer is stored

    ///< These are used to store the selections from the GLUI user interface
    int m_curTool;
    std::string m_fileName;
    std::string m_nextFileName;
    std::string m_prevFileName;

    Tool* tool[2];  ///< An array of pointers to the Tools used by MIA
    ImageHandler* imageHandler; ///< Points to the location where MIA's ImageHandler is stored
    FilterHandler* m_filterHandle; ///< Points to the location where MIA's FilterHandler is stored
    PixelBufferHistory* dspHistory; ///< Points to the PixelBufferHistory used to implement undo/redo in MIA
};


#endif
