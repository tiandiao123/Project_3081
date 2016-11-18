#include "CLHandler.h"
#include "ImageHandler.h"
#include "FilterHandler.h"
#include "PixelBuffer.h"
#include "ColorData.h"

using std::cout;
using std::endl;
using std::string;


CLHandler::CLHandler() {
    imageHandler = new ImageHandler();
    filterHandler = new FilterHandler();
    buf = new PixelBuffer(1,1,ColorData(0,0,0,0));
    testing = 0;
}

CLHandler::~CLHandler() {}

void CLHandler::quitProgram()
{
    if (testing) { throw(0); }
    else { displayHelp(); exit(1); }
}

void CLHandler::compare(const std::string & file2) {
    if (isValidImageFile(file2)) {
        PixelBuffer *buf2 = new PixelBuffer(1,1,ColorData(0,0,0,0));
        if (imageHandler->loadImage(file2, buf2)) {
            // Check if both images are the same
            bool same = true;
            int x,y;
            if((buf->getWidth() == buf2->getWidth()) && (buf->getHeight() == buf2->getHeight())) {
                for(y=0; y<buf->getHeight(); y++) {
                    for(x=0; x<buf->getWidth(); x++) {
                        ColorData bufPixel = buf->getPixel(x,y);
                        ColorData buf2Pixel = buf2->getPixel(x,y);
                        if(bufPixel != buf2Pixel) { same = false; }
                    }
                }
            }
            else { same = false; }

            if(same) { std::cout << "1" << std::endl; }
            else { std::cout << "0" << std::endl; }
        }
        else {
            std::cout << "Unable to load second image - " << file2 << "\n" << std::endl;
            quitProgram();
        }
    }
    else {
        std::cout << "Second file is not a valid image - " << file2 << "\n" << std::endl;
        quitProgram();
    }
}

void CLHandler::applyNextFilter(char** commands, int &arg_i) {
    if (!(strcmp(commands[arg_i], "-edgedetect"))) {
        filterHandler->apply(buf, EDGEDETECTION, {});
    }
    else if (!(strcmp(commands[arg_i], "-sharpen"))) {
        float args[1] = {atof(commands[++arg_i])};
        if (args[0] < 0 || args[0] > 100) {
            std::cout << "Invalid value" << "\n" <<std::endl;
            quitProgram();
        }
        filterHandler->apply(buf, SHARPEN, args);
    }
    else if(!(strcmp(commands[arg_i], "-thresh"))) {
        float args[1] = {atof(commands[++arg_i])};
        if (args[0] < 0.0 || args[0] > 1.0) {
            std::cout << "Invalid value" << "\n" <<std::endl;
            quitProgram();
        }
        filterHandler->apply(buf, THRESHOLD, args);
    }
    else if(!(strcmp(commands[arg_i], "-quantize"))) {
        float args[1] = {atof(commands[++arg_i])};
        if (args[0] < 2 || args[0] > 256) {
            std::cout << "Invalid value" << "\n" <<std::endl;
            quitProgram();
        }
        filterHandler->apply(buf, QUANTIZE, args);
    }
    else if(!(strcmp(commands[arg_i], "-blur"))) {
        float args[1] = {atof(commands[++arg_i])};
        if (args[0] < 0 || args[0] > 20) {
            std::cout << "Invalid value" << "\n" <<std::endl;
            quitProgram();
        }
        filterHandler->apply(buf, BLUR, args);
    }
    else if(!(strcmp(commands[arg_i], "-saturate"))) {
        float args[1] = {atof(commands[++arg_i])};
        if (args[0] < -10.0 || args[0] > 10.0) {
            std::cout << "Invalid value" << "\n" <<std::endl;
            quitProgram();
        }
        filterHandler->apply(buf, SATURATION, args);
    }
    else if(!(strcmp(commands[arg_i], "-multrgb"))) {
        float args[3];
        char *token = strtok(commands[++arg_i],",");
        int i = 0;
        while (token != NULL) {
            args[i] = atof(token);
            if (args[i] < 0.0 || args[i] > 10.0) {
                std::cout << "Invalid value" << "\n" <<std::endl;
                quitProgram();
            }
            i++;
            token = strtok(NULL,",");
        }
        filterHandler->apply(buf, RGB, args);
    }
    else {
        std::cout << "Invalid argument" << "\n" << std::endl;
        quitProgram();
    }

    arg_i++;
}

void CLHandler::displayHelp() {
    std::cout << "\nTo use the MIA app command line interface, simply type the program name './Mia' followed by the appropriate arguments" << std::endl;
    std::cout << "- To display this help message, enter './Mia -h'" << std::endl;
    std::cout << "- To simply copy an image file to another image file, enter the original image followed by the copy: './Mia original.jpg copy.png'" << std::endl;
    std::cout << "- To compare two images to see if they are identical, use the -compare option between the image files: './Mia image1.jpg -compare image2.jpg'" << std::endl;
    std::cout << "- To apply a filter to an image, enter the source image followed by the filter and its arguments, then the destination image (see below for use of each filter)\n" << std::endl;
    std::cout << "-- Edgedetect: './Mia source.jpg -edgedetect destination.jpg'" << std::endl;
    std::cout << "--    Sharpen: './Mia source.jpg -sharpen <integer> destination.jpg', where 0 < integer < 100" << std::endl;
    std::cout << "--  Threshold: './Mia source.jpg -thresh <float> destination.jpg', where 0 < float < 1" << std::endl;
    std::cout << "--   Quantize: './Mia source.jpg -quantize <integer> destination.jpg', where 2 < integer < 256" << std::endl;
    std::cout << "--       Blur: './Mia source.jpg -blur <integer> destination.jpg', where 0 < integer < 20" << std::endl;
    std::cout << "--   Saturate: './Mia source.jpg -saturate <float> destination.jpg', where -10 < float < 10" << std::endl;
    std::cout << "-- RGBchannel: './Mia source.jpg -multrgb <R float>,<G float>,<B float> destination.jpg', where for each float, 0 < float < 10\n" << std::endl;
    std::cout << "Multiple filters can be applied to a single image by sequencing them: './Mia source.jpg -sharpen 5 -blur 1 destination.jpg'" << std::endl;
    std::cout << "Filters can be applied to every image in a directory by specifying directories instead of image files: './Mia inDirectory -edgedetect outDirectory'\n" << std::endl;
}

int CLHandler::handleCommand(int argc, char** argv) {
    std::string save = "";
    bool toSave = false;
    int arg_i = 1;

    // Check 1st arg
    if (!(strcmp(argv[arg_i], "-h"))) {
        displayHelp();
    }
    else if (isValidImageFile(argv[arg_i])) {
        // Load image
        if (imageHandler->loadImage(argv[arg_i], buf)) {
            arg_i++;
            // Handle next args
            while(arg_i < argc) {
                if (isValidImageFileName(argv[arg_i])) {
                    save = argv[arg_i];
                    toSave = true;
                    break;
                }
                // else if (isDirectory) {
                //     // Handle directory
                // }
                else if (!(strcmp(argv[arg_i], "-compare"))) {
                    compare(argv[++arg_i]);
                    break;
                }
                else { applyNextFilter(argv, arg_i); }
            }
        }
        else {
            std::cout << "Unable to load image " << argv[arg_i] << "\n" << std::endl;
            quitProgram();
        }

        // Save image if necessary
        if(toSave && !(imageHandler->saveImage(save, buf))) {
            std::cout << "Unable to save to image " << save << "\n" << std::endl;
            quitProgram();
        }
    }
    else if ((stat(argv[arg_i],&st_buf)==0)&&  S_ISDIR(st_buf.st_mode)) {
         // Handle directory
         DIR* FD;

         std::cout<<"first one"<<endl;
         struct dirent* in_file;
         if (NULL == (FD = opendir (argv[1])))
         {
              cout<<"failed to open dir"<<endl;
              quitProgram();
         }


         while ((in_file = readdir(FD))!=NULL){
		 	 //std::cout<<"second one"<<endl;
		 	 arg_i=1;
		     std::string strimagefile(in_file->d_name);
			 std::cout<<strimagefile<<endl;

			 if (isValidImageFileName(strimagefile)){

				const int num=argc;
				char file1[50]="";
				strcat(file1,argv[arg_i]);
				strcat(file1,"/");
				strcat(file1,strimagefile.c_str());
				imageHandler->loadImage(file1, buf);


				++arg_i;
                while (arg_i<argc-1){
				applyNextFilter(argv, arg_i);
				}

				std::string strtemp(argv[argc-1]);
				save=strtemp+"/"+strimagefile;

				imageHandler->saveImage(save, buf);

		      }
	      }



      }
    else {
        std::cout << "First argument is invalid" << "\n" << std::endl;
        quitProgram();
    }

    return 0;
}


/* Definitions of helpful functions copied from MIAApp */
bool CLHandler::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}

bool CLHandler::isValidImageFileName(const std::string & name) {

    if (hasSuffix(name, ".png")
        || hasSuffix(name, ".jpg")
        || hasSuffix(name, ".jpeg")
        )
        return true;
    else
        return false;
}

bool CLHandler::isValidImageFile(const std::string & name) {

    FILE *f;
    bool isValid = false;
    if (isValidImageFileName(name)) {
        if ((f = fopen( name.c_str(), "r"))) {
            isValid = true;
            fclose(f);
        }
    }


    return isValid;
}

std::string CLHandler::getImageNamePlusSeqOffset(const std::string & currentFileName, int offset)
{

    int digitCount = 3;

    std::string extension, name, number;
    size_t dotPos = currentFileName.find_last_of(".");
    if (dotPos ==  std::string::npos || dotPos == 0) {
        return "";
    }

    extension = currentFileName.substr(dotPos+1);
    name = currentFileName.substr(0,dotPos);
    if (name.length() < digitCount) {
        return "";
    }

    number = name.substr(name.length()-digitCount);
    name = name.substr(0,name.length()-digitCount);
    int num;
    std::istringstream ( number ) >> num;
    int output_num = num +offset;
    if (output_num < 0) {
        return "";
    }
    std::stringstream ss;
    ss << output_num;
    std::string output_number;
    ss >> output_number;

    // Append zero chars
    size_t str_length = output_number.length();
    for (int i = 0; i < digitCount - str_length; i++)
        output_number = "0" + output_number;





    return (name + output_number + "." + extension);
}
