//
//  main.cpp
//  Originally created by the CSci-3081W TAs.
//


#include "FlashPhotoApp.h"
#include "ColorData.h"

int main(int argc, char* argv[]) {
    FlashPhotoApp *app = new FlashPhotoApp(argc, argv, 800, 800, ColorData(1,1,0.95));
    // runMainLoop returns when the user closes the graphics window.
    app->runMainLoop();
    delete app;
    exit(0);
}
