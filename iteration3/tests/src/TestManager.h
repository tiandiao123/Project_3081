#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "PixelBuffer.h"
#include "FlashPhotoApp.h"
#include "ColorData.h"
#ifndef IMAGEHAND_H
#include "ImageHandler.h"
#endif
#include "CLHandler.h"

// Pathing for test images
const std::string BASE_IMG_PATH_NOEXT("./img/base");
const std::string TOOL_TEST_PATH_NOEXT("./img/tooltest");
const std::string FILT_TEST_PATH_NOEXT("./img/filttest");
const std::string TOOL_RES_PATH_NOEXT("./img/toolres");
const std::string FILT_RES_PATH_NOEXT("./img/filtres");
const std::string JPG_EXT("jpg");
const std::string PNG_EXT("png");

// Test result error codes
#define SIZE_MISMATCH_FAIL 1
#define PIXEL_MISMATCH_FAIL 3
#define UNKNOWN_FAIL 10

#define MISMATCH_THRESHOLD 10 // Amount of pixels that can differ before returning a PIXEL_MISMATCH_FAIL

class FlashPhotoApp;

class TestManager {
public:
    TestManager(int argc, char* argv[]);
    ~TestManager();
    int testTools(int nStart, int nEnd);
    int testFilters(int nStart, int nEnd);
    int testCLI();

private:
    int testTools_format(int nStart, int nEnd, std::string format);
    int testFilters_format(int nStart, int nEnd, std::string format);
    int runTool(int idx, int height);
    int runFilter(int idx);
    int compareFilesByPixelBuffer(std::string a, std::string b);
    int compareBuffers(PixelBuffer *a, PixelBuffer *b);

    std::string itos(int i);

    FlashPhotoApp *app_fp;
    CLHandler *cl_handle;
    ImageHandler *img_handle;
};
