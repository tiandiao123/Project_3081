## A Makefile for LibPhoto

UNAME = $(shell uname)
CC 		= g++
CFLAGS 	= -g -c -Wall
LDFLAGS = -g

LIBRARY  = libphoto.a
LAR      = ar
LFLAGS   = -rcs

PNG = png
JPEG = jpeg
GLUI = glui

JPEG_PATH = ../libjpeg
PNG_PATH = ../libpng
GLUI_PATH = ../glui

PNG_LIB = $(PNG_PATH)/lib/lib$(PNG).a
JPEG_LIB = $(JPEG_PATH)/lib/lib$(JPEG).a
GLUI_LIB = $(GLUI_PATH)/lib/lib$(GLUI).a

INCLUDE_PNG = -I$(PNG_PATH)/include
INCLUDE_JPEG = -I$(JPEG_PATH)/include
INCLUDE_GLUI = -I$(GLUI_PATH)/include

OBJECT_DIR = object_files
SOURCE_DIR = src
LIB_DIR    = lib
INC_DIR    = include

LINK_LIBS +=  -L$(PNG_PATH)/lib/ -l$(PNG)
LINK_LIBS +=  -L$(JPEG_PATH)/lib/ -l$(JPEG)
LINK_LIBS +=  -lz

OBJECTS = $(addprefix $(OBJECT_DIR)/,  $(FILENAMES:.cpp=.o))
CPPFILES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERFILES = $(wildcard $(SOURCE_DIR)/*.h)
LINKEDINCS = $(addprefix $(INC_DIR)/, $(notdir $(HEADERFILES)))
FILENAMES =  $(notdir $(CPPFILES))

build: setup $(LIB_DIR)/$(LIBRARY)

rebuild: clean build

# Create the object_files directory only if it does not exist.
setup: | $(OBJECT_DIR) $(LIB_DIR) $(INC_DIR) $(LINKEDINCS)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(INC_DIR):
	mkdir -p $(INC_DIR)

# Archive the object files into a library
# Copy headers for inclusion?

$(LIB_DIR)/$(LIBRARY): setup $(PNG_LIB) $(JPEG_LIB) $(OBJECTS)
	$(LAR) $(LFLAGS) $@ $(OBJECTS)

# Include directory

$(INC_DIR)/%.h:
	ln -s $(abspath $(SOURCE_DIR)/$(notdir $@)) $(abspath $(INC_DIR))

# Explicitly defined build targets because of dependancy on other files

$(OBJECT_DIR)/ImageHandler.o: $(SOURCE_DIR)/ImageHandler.cpp $(SOURCE_DIR)/ImageHandler.h $(PNG_LIB) $(JPEG_LIB)
	$(CC) $(CFLAGS) $(INCLUDE_JPEG) $(INCLUDE_PNG) -o $@ $<

$(OBJECT_DIR)/BaseGfxApp.o: $(SOURCE_DIR)/BaseGfxApp.cpp $(SOURCE_DIR)/BaseGfxApp.h $(GLUI_LIB)
	$(CC) $(CFLAGS) $(INCLUDE_GLUI)  -o $@ $<

# Pattern build targets
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.h
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $<

#include $(LINK_LIBS) ?

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(LINK_LIBS) -o $@ $<

clean:
	\rm -rf $(OBJECT_DIR) $(LIBRARY) $(INC_DIR) $(LIB_DIR)

cleandeps: clean cleanjpeg cleanpng cleanglui

cleanjpeg:
	$(MAKE) -C ./$(JPEG_PATH) clean
	cd $(JPEG_PATH); \
	rm -rf lib bin include share

cleanpng:
	$(MAKE) -C ./$(PNG_PATH) clean
	cd $(PNG_PATH); \
	rm -rf lib bin include share

cleanglui:
	$(MAKE)  -C ./$(GLUI_PATH)/ clean

$(PNG_LIB):
	cd $(PNG_PATH); \
	./configure --enable-shared=no  --prefix=$(abspath $(PNG_PATH))
	$(MAKE) -C $(PNG_PATH)
	# $(MAKE) -C $(PNG_PATH) check
	$(MAKE) -C $(PNG_PATH) install

$(JPEG_LIB):
	cd $(JPEG_PATH); \
	./configure --enable-shared=no --prefix=$(abspath $(JPEG_PATH))
	$(MAKE) -C $(JPEG_PATH)
	$(MAKE) -C $(JPEG_PATH) check
	$(MAKE) -C $(JPEG_PATH) install

$(GLUI_LIB):
	$(MAKE) -C $(GLUI_PATH) all
