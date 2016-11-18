#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include "TestManager.h"


TestManager::TestManager(int argc, char* argv[]) {
    app_fp = new FlashPhotoApp(argc, argv, 800, 800, ColorData(1,1,0.95));
    cl_handle = new CLHandler();
    img_handle = new ImageHandler();
    cl_handle->testing = 1;
}

TestManager::~TestManager() {
    delete app_fp;
    delete cl_handle;
    delete img_handle;
}

int TestManager::testTools(int nStart, int nEnd) {
    int res = 0;
    if (nEnd == -1) nEnd = TOOL_COUNT;
    res += testTools_format(nStart, nEnd, JPG_EXT);
    res += testTools_format(nStart, nEnd, PNG_EXT);
    return res;
}

int TestManager::testTools_format(int nStart, int nEnd, std::string format) {
    app_fp->setImageFile(BASE_IMG_PATH_NOEXT + "." + format);
    app_fp->loadImageToCanvas();
    PixelBuffer *buf = app_fp->getBuffer();
    for (int i = nStart; i < nEnd; ++i)
        runTool(i,  buf->getHeight()/(nEnd - nStart));
    app_fp->setImageFile(TOOL_TEST_PATH_NOEXT + "." + format);
    app_fp->saveCanvasToFile();

    // app_fp->setImageFile(TOOL_RES_PATH_NOEXT + "." + format);
    // app_fp->saveCanvasToFile(); // For generation of result comparation files, remove later

    return compareFilesByPixelBuffer(TOOL_TEST_PATH_NOEXT + "." + format, TOOL_RES_PATH_NOEXT + "." + format);
}

int TestManager::testFilters(int nStart, int nEnd) {
    int res = 0;
    res += testFilters_format(nStart, nEnd, JPG_EXT);
    res += testFilters_format(nStart, nEnd, PNG_EXT);
    return res;
}

int TestManager::testFilters_format(int nStart, int nEnd, std::string format) {
    int res = 0;
    for (int i = nStart; i < nEnd; ++i) {
        app_fp->setImageFile(BASE_IMG_PATH_NOEXT + "." + format);
        app_fp->loadImageToCanvas();
        runFilter(i);
        app_fp->setImageFile(FILT_TEST_PATH_NOEXT + itos(i) + "." + format);
        app_fp->saveCanvasToFile();

        // app_fp->setImageFile(FILT_RES_PATH_NOEXT + itos(i) + "." + format);
        // app_fp->saveCanvasToFile(); // For generation of result comparation files, remove later

        res += compareFilesByPixelBuffer(FILT_TEST_PATH_NOEXT + itos(i) + "." + format, FILT_RES_PATH_NOEXT + itos(i) + "." + format);
    }
    return res;
}

int TestManager::runTool(int idx, int incHeight) {
    app_fp->setCurTool(idx);
    std::cout << "Testing tool " << idx << std::endl;
    PixelBuffer *buf = app_fp->getBuffer();
    int height = (idx * incHeight);
    if (idx == 0) height += incHeight/3;
    if (idx == TOOL_COUNT) height -= incHeight/3;
    app_fp->leftMouseDown(0, height);
    for (int i = 0; i < buf->getWidth(); i += 5)
        app_fp->mouseDragged(i, height);
    app_fp->leftMouseUp(0, 0);
}

int TestManager::runFilter(int idx) {
    int init_idx = app_fp->UI_APPLY_BLUR;
    idx += init_idx;
    app_fp->gluiControl(idx);
}

int TestManager::compareFilesByPixelBuffer(std::string a, std::string b) {
    PixelBuffer *a_buf, *b_buf;
    ColorData *bg = new ColorData(0,0,0,1);
    img_handle->loadImage(a, a_buf, *bg);
    img_handle->loadImage(b, b_buf, *bg);
    int res = compareBuffers(a_buf, b_buf);
    // delete a_buf, b_buf, bg;
    return res;
}

int TestManager::compareBuffers(PixelBuffer *a, PixelBuffer *b) {
    int pixel_mismatch = 0;
    if (a->getWidth() != b->getWidth() || a->getHeight() != b->getHeight()) return SIZE_MISMATCH_FAIL;
    for (int i = 0; i < a->getWidth(); ++i)
        for (int j = 0; j < a->getHeight(); ++j)
            if ((a->getPixel(i, j) != b->getPixel(i, j))) pixel_mismatch++;
    if (pixel_mismatch > MISMATCH_THRESHOLD) return PIXEL_MISMATCH_FAIL;
    return 0;
}

std::string TestManager::itos(int i) // convert int to string
{
    std::stringstream s;
    s << i;
    return s.str();
}

int TestManager::testCLI() {
    int res = 0;
    int argc;
    char* argv[20];

    // Test valid inputs
    argc = 4;
    argv = {"./Mia", "./img/base.jpg", "-edgedetect", "test0.jpg"};
    res += cl_handle->handleCommand(argc, argv);

    argc = 5;
    argv = {"./Mia", "./img/base.jpg", "-blur", "5", "test0.jpg"};
    res += cl_handle->handleCommand(argc, argv);

    argc = 8;
    argv = {"./Mia", "./img/base.jpg", "-blur", "5", "-sharpen", "5", "-edgedetect", "test0.jpg"};
    res += cl_handle->handleCommand(argc, argv);

    argc = 4;
    argv = {"./Mia", "./img/base.jpg", "-compare", "test0.jpg"};
    res += cl_handle->handleCommand(argc, argv);

    argc = 5;
    argv = {"./Mia", "./img", "-blur", "1", "./test"};
    res += cl_handle->handleCommand(argc, argv);

    //Test invalid inputs
    argc = 4;
    argv = {"./Mia", "./img/base.jpg", "-blur", "test0.jpg"};
    try { cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 5;
    argv = {"./Mia", "./img/base.jpg", "-edgedetect", "5", "test0.jpg"};
    try { cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 4;
    argv = {"./Mia", "./img/base.jpg", "compare", "test0.jpg"};
    try { cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 3;
    argv = {"./Mia", "./img/base.jpg", "-compare"};
    try { cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 3;
    argv = {"./Mia", "./img/base.jpg"};
    try { cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 5;
    argv = {"./Mia", "./img/base.jpg", "-blur", "30", "test0.jpg"};
    try { res += cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    argc = 5;
    argv = {"./Mia", "./img/base.jpg", "-sharpen", "-1", "test0.jpg"};
    try { res += cl_handle->handleCommand(argc, argv); }
    catch(...) { res = res + 0; }

    return res;
}
