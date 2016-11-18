//
//  BaseGfxApp.h
//  Originally created by the CSci-3081W TAs.
//

#ifndef BASEGFXAPP_H
#define BASEGFXAPP_H

#include <string>

// The GLUI library, which in turn will include gl.h and glut.h
#include <GL/glui.h>


/** This is a base class for graphics applications built on top of the GLUT and GLUI toolkits.
    GLUT and GLUI are C libraries, so one function of this class is to wrap the funcationality
    they provide in a class structure that lends itself to C++.  To receive callbaks from
    GLUT and GLUI that allow you to render graphics and respond to user interface events,
    simply override the virtual methods in this class within your own subclass. */
class BaseGfxApp
{
public:
    BaseGfxApp(int argc, char* argv[], int width, int height, int x, int y, int glutFlags,
        bool createGLUIWin, int gluiWinX, int gluiWinY);
    virtual ~BaseGfxApp();

    void setCaption(const std::string& caption);
    void setWindowDimensions(int width, int height);

    /// Call this function to start the program.  It will not return until the graphics window is closed.
    void runMainLoop();

    /// Subclasses should override this method in order to draw graphics to the screen.
    virtual void display() {};

    /// This is a convenience function that draws an array of pixel data to the screen.
    void drawPixels(int start_x, int start_y, int width, int height, void const * const pixels);

    /// It can be useful to override this method when doing animation.  The argument tells you the time
    /// since the last redraw of the screen.
    virtual void update(int delta_time_ms) {};

    /// The following functions provide callbacks for user interface events in the GLUT window.
    virtual void mouseMoved(int x, int y) {};
    virtual void mouseDragged(int x, int y) {};

    virtual void leftMouseDown(int x, int y) {};
    virtual void leftMouseUp(int x, int y) {};
    virtual void rightMouseDown(int x, int y) {};
    virtual void rightMouseUp(int x, int y) {};
    virtual void middleMouseDown(int x, int y) {};
    virtual void middleMouseUp(int x, int y) {};

    virtual void keyboard(unsigned char c, int x, int y) {};
    virtual void keyboardSpecial(int key, int x, int y) {};
    virtual void keyboardUp(unsigned char c, int x, int y) {};
    virtual void keyboardSpecialUp(int key, int x, int y) {};

    /// This is the callback that tells you when the user has interacted with a GLUI widget
    virtual void gluiControl(int controlID) {};


    /// Glut calls this when the user resizes the graphics window by dragging on it with the mouse.
    /// The default implementation in BaseGfxApp is to ignore this interaction and force the window
    /// to remain the same size.  But, if desired, subclasses can override this method and implement
    /// the logic to allow for interactive resizing of windows.
    virtual void reshape(int width, int height);

    /// Clears the screen and calls display(). Subclasses would only need to override this method if they
    /// need direct control over clearing the screen, which is usually not the case.
    virtual void renderOneFrame();

    int width() const;
    int height() const;
    int handle() { return m_glutWindowHandle; }
    GLUI* glui() { return m_glui; }

protected:

    /* GLUT and GLUI event callbacks are sent to the current window/app. Right now, there is only
     one window anyway (not counting the GLUI UI window.. in the future could be extended to
     support more windows.  In any case, some structure like this is always needed when using
     glut with C++, since the glut callbacks must be either global or static functions. */
    static BaseGfxApp *s_currentApp;

    // Has glutInit been called? (only allowed once per program)
    static bool s_glutInitialized;

    // Glut callbacks:
    static void s_reshape(int width, int height);
    static void s_keyboard(unsigned char c, int x, int y);
    static void s_keyboardspecial(int key, int x, int y);
    static void s_keyboardup(unsigned char c, int x, int y);
    static void s_keyboardspecialup(int key, int x, int y);
    static void s_mousemotion(int x, int y);
    static void s_mousebtn(int b, int s, int x, int y);
    static void s_draw();
    static void s_gluicallback(int controlID);
    static void s_idle();

    /// Underlying glut window handle
    int m_glutWindowHandle;

    /// Pointer to GLUI master
    GLUI *m_glui;

    bool m_drag;
    int m_width;
    int m_height;
    int m_milliseconds;
};


#endif
