//
//  FlashPhotoApp.h
//  Originally created by the CSci-3081W TAs.
//

#ifndef FLASHPHOTOAPP_H
#define FLASHPHOTOAPP_H

#include "BaseGfxApp.h"
#include "Tool.h"
#include "ImageHandler.h"
#include "FilterHandler.h"
#include "PixelBufferHistory.h"

#define TOOL_COUNT 7


class ColorData;
class PixelBuffer;

class FlashPhotoApp : public BaseGfxApp {
public:

    /// Executes all the initialization functions and starts the GUI
    FlashPhotoApp(int argc, char* argv[], int width, int height, ColorData backgroundColor);
    virtual ~FlashPhotoApp(); /// Cleans up the display buffer and exits

    /// Mouse moved with left mouse button down
    void mouseDragged(int x, int y) ;
    void mouseMoved(int x, int y);
    void leftMouseDown(int x, int y);
    void leftMouseUp(int x, int y);
    void display();
    void gluiControl(int controlID);

    PixelBuffer* getBuffer();
    /// Attempts to load an image to the current PixelBuffer, adjusting GLUI dimensions, clears history
    void loadImageToCanvas();
    /// Attempts to load an image to the stamp tool
    void loadImageToStamp();
    /// Saves the current PixelBuffer to the current file name
    void saveCanvasToFile();
    /// Pushes an item to the PixelBufferHistory
    void storeCanvasState();
    /// Updates the current file name target
    void setImageFile(const std::string & filepath);
    /// Updates the current file tool
    void setCurTool(int x);

    /// GLUI INTERFACE ELEMENTS
    enum UIControlType {
        UI_TOOLTYPE,
        UI_COLOR_R,
        UI_COLOR_G,
        UI_COLOR_B,
        UI_PRESET_RED,
        UI_PRESET_ORANGE,
        UI_PRESET_YELLOW,
        UI_PRESET_GREEN,
        UI_PRESET_BLUE,
        UI_PRESET_PURPLE,
        UI_PRESET_WHITE,
        UI_PRESET_BLACK,
        UI_FILE_BROWSER,
        UI_LOAD_CANVAS_BUTTON,
        UI_LOAD_STAMP_BUTTON,
        UI_SAVE_CANVAS_BUTTON,
        UI_FILE_NAME,
        UI_APPLY_BLUR,
        UI_APPLY_SHARP,
        UI_APPLY_EDGE,
        UI_APPLY_THRESHOLD,
        UI_APPLY_SATURATE,
        UI_APPLY_CHANNEL,
        UI_APPLY_QUANTIZE,
        UI_APPLY_MOTION_BLUR,
        UI_APPLY_SEPIA,
        UI_APPLY_VIGNETTE,
        UI_UNDO,
        UI_REDO,
        UI_QUIT
    };

private:

    bool isValidImageFileName(const std::string & name);
    bool isValidImageFile(const std::string & name);
    bool hasSuffix(const std::string & str, const std::string & suffix);
    void buttonEnabled(GLUI_Button * button, bool enabled); ///< Toggles access to GLUI button
    void undoEnabled(bool enabled);
    void redoEnabled(bool enabled);
    void saveCanvasEnabled(bool enabled);
    void loadCanvasEnabled(bool enabled);
    void loadStampEnabled(bool enabled);
    void updateColors();

    void applyFilterBlur();
    void applyFilterSharpen();
    void applyFilterMotionBlur();
    void applyFilterEdgeDetect();
    void applyFilterThreshold();
    void applyFilterChannel();
    void applyFilterSaturate();
    void applyFilterQuantize();
    void applyFilterSepia();
    void applyFilterVignette();
    void undoOperation();
    void redoOperation();


    /// Builds the GLUI objects (panels, buttons, etc)
    void initGlui();
    /// Initialize OpenGL for 2D graphics as used in the app
    void initGraphics();
    /// Creates current display buffer object and display buffer history
    void initializeBuffers(ColorData initialColor, int width, int height);
    /// Builds all tools for the tool array
	void initializeTools();
    /// Initializes a ImageHandler object
    void initializeImageLibs();
    /// Initialzes a FilterHandler object
    void initializeFilters();

    enum UIMotionBlurDirections {
        DIR_N_S,
        DIR_E_W,
        DIR_NE_SW,
        DIR_NW_SE
    };

    /// Holds arguments for the filters updated by GLUI
    struct {
        float channel_colorRed;
        float channel_colorGreen;
        float channel_colorBlue;
        float saturation_amount;
        float threshold_amount;
        float blur_amount;
        float sharpen_amount;
        float motionBlur_amount;
        float vignette_amount;
        int motionBlur_direction;
        int quantize_bins;
    } m_filterParameters;

    struct {
        GLUI_FileBrowser* fileBrowser;
        GLUI_Button *loadCanvasButton;
        GLUI_Button *loadStampButton;
        GLUI_Button *saveCanvasButton;
        GLUI_Button *redoButton;
        GLUI_Button *undoButton;
        GLUI_StaticText * currentFileLabel;
        GLUI_EditText* fileNameBox;
        GLUI_StaticText * saveFileLabel;

        GLUI_Spinner *spinnerRed;
        GLUI_Spinner *spinnerGreen;
        GLUI_Spinner *spinnerBlue;
    } m_gluiControlHooks;


    /// This is the pointer to the buffer where the display PixelBuffer is stored
    PixelBuffer* m_displayBuffer;

    // These are used to store the selections from the GLUI user interface
    int m_curTool;
    float m_curColorRed, m_curColorGreen, m_curColorBlue;
    std::string m_fileName;

	Tool* tool[TOOL_COUNT]; ///< Stores the Tool objects in an array for application
    ImageHandler* imageHandler;
    FilterHandler* m_filterHandle;
    PixelBufferHistory* dspHistory;
};


#endif
