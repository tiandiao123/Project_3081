#include <sstream>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

using std::cout;
using std::endl;
using std::string;

class ImageHandler;
class FilterHandler;
class PixelBuffer;
#ifndef CLHANDLER_H
#define CLHANDLER_H
/** The CLHandler class is instantiated and used whenever command-line options are specified for MIA.
 The only public function besides the constructor/destructor is handleCommand, which is called by main.cpp to do all the work of handling command-line input.
*/
class CLHandler {
public:
    CLHandler();
    ~CLHandler();

    int handleCommand(int argc, char** argv); ///< This function is called whenever command line options are specified for Mia
    int testing;

private:
    ImageHandler *imageHandler; ///< ImageHandler used by CLHandler to load/save images
    FilterHandler *filterHandler; ///< FilterHandler used by CLHandler to apply filters to images
    PixelBuffer *buf; ///< PixelBuffer used to temporarily load images
    struct stat st_buf; ///< Used to check if input is a directory

    void compare(const std::string &); ///< Used when the -compare option is specified on the command line; compares two images to see if they're the same
    void applyNextFilter(char* commands[], int &arg_i); ///< Applies the next filter specified on the command line
    void displayHelp(); ///< Prints out the help message when the -h options is specified or an error has occurred
    void quitProgram(); ///< Used to determine what to do if an error is encountered

    /** Helpful functions coppied from MIAApp */
    bool isValidImageFileName(const std::string & name);
    bool isValidImageFile(const std::string & name);
    bool hasSuffix(const std::string & str, const std::string & suffix);
    std::string getImageNamePlusSeqOffset(const std::string & currentFileName, int offset);
};
#endif
