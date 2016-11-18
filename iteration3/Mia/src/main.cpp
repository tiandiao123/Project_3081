//
//  main.cpp
//  Originally created by the CSci-3081W TAs.
//


#include "MIAApp.h"
#include "ColorData.h"
#include "CLHandler.h"

int main(int argc, char* argv[]) {
    // Determine if command line options were specified; if not, start app as normal
    if (argc > 1) {
        CLHandler *clhandler = new CLHandler();
        clhandler->handleCommand(argc, argv);
    }
    else {
        MIAApp *app = new MIAApp(argc, argv, 800, 800, ColorData(1,1,0.95));
        // runMainLoop returns when the user closes the graphics window.
        app->runMainLoop();
        delete app;
    }

    exit(0);
}
