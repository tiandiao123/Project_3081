## A Makefile for tests
# Created by CSCI3081W Guru
# Copyright (c) CSCI3081W Guru. All rights reserved.

# Heavily modified by adkin099

UNAME = $(shell uname)
CC 		= g++
CFLAGS 	= -g -c -Wall
LDFLAGS = -g

ifeq ($(UNAME), Darwin) # Mac OSX
	LINK_LIBS += -framework glut -framework opengl
	EXECUTABLE = testex
else # LINUX
	LINK_LIBS +=  -lglut -lGL -lGLU
	EXECUTABLE = testex
endif

GLUI = glui
PNG = png
JPEG = jpeg
PHOTO = photo
FLASH = FlashPhoto
MIA = Mia

GLUI_PATH = ../glui
JPEG_PATH = ../libjpeg
PNG_PATH = ../libpng
PHOTO_PATH = ../libphoto
FLASH_PATH = ../FlashPhoto
MIA_PATH = ../Mia

REL_PATH = ../tests
IMG_PATH = ./img
REL_IMG = $(REL_PATH)/$(IMG_PATH)

FLASH_EXEC = $(FLASH_PATH)/$(FLASH)
MIA_EXEC = $(MIA_PATH)/$(MIA)

GLUI_LIB = $(GLUI_PATH)/lib/lib$(GLUI).a
PNG_LIB = $(PNG_PATH)/lib/lib$(PNG).a
JPEG_LIB = $(JPEG_PATH)/lib/lib$(JPEG).a
PHOTO_LIB = $(PHOTO_PATH)/lib/lib$(PHOTO).a


INCLUDE += -I$(GLUI_PATH)/include
INCLUDE += -I$(PNG_PATH)/include
INCLUDE += -I$(JPEG_PATH)/include
INCLUDE += -I$(PHOTO_PATH)/include
INCLUDE += -I$(FLASH_PATH)/src
INCLUDE += -I$(MIA_PATH)/src

OBJECT_DIR = object_files
SOURCE_DIR = src

LINK_LIBS +=  -L$(PHOTO_PATH)/lib/ -l$(PHOTO)
LINK_LIBS +=  -L$(PNG_PATH)/lib/ -l$(PNG)
LINK_LIBS +=  -L$(JPEG_PATH)/lib/ -l$(JPEG)
LINK_LIBS +=  -L$(GLUI_PATH)/lib/ -l$(GLUI)
LINK_LIBS +=  -lz

# Include nvidia drivers if necessary:
#NVIDIA_LIB =  /usr/lib/nvidia-304 #or whichever nvidia number shows up in that dir
ifneq ($(NVIDIA_LIB), )
	LINK_LIBS += -L$(NVIDIA_LIB)
endif

OBJECTS = $(addprefix $(OBJECT_DIR)/,  $(FILENAMES:.cpp=.o))
CPPFILES = $(wildcard $(SOURCE_DIR)/*.cpp)
FILENAMES =  $(notdir $(CPPFILES))

tests: fphcli_tests mia_tests
	@echo All tests completed

build: | setup $(EXECUTABLE)

rebuild: clean build

all: tests


# Create the object_files directory only if it does not exist.
setup: | $(OBJECT_DIR)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(EXECUTABLE): | setup $(GLUI_LIB) $(PNG_LIB) $(JPEG_LIB) $(PHOTO_LIB) $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(FLASH_PATH)/object_files/FlashPhotoApp.o $(MIA_PATH)/object_files/CLHandler.o $(LINK_LIBS) -o $@

#Explicit build
$(OBJECT_DIR)/TestManager.o: $(SOURCE_DIR)/TestManager.cpp $(SOURCE_DIR)/TestManager.h $(FLASH_PATH)/src/FlashPhotoApp.h $(MIA_PATH)/src/CLHandler.h
	#'Compiling arbitrary .cpp file with .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

# Pattern build targets
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(SOURCE_DIR)/%.h
	#'Compiling arbitrary .cpp file with .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	#'Compiling arbitrary .cpp file without .h'
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

fphcli_tests: $(EXECUTABLE) $(FLASH_EXEC)
	./$(EXECUTABLE)
	@sleep 5

mia_tests: $(MIA_EXEC)
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -blur 5 $(IMG_PATH)/miafilttest0.png
	diff $(IMG_PATH)/miafilttest0.png $(IMG_PATH)/filtres0.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -blur 5 $(IMG_PATH)/miafilttest0.jpg
	diff $(IMG_PATH)/miafilttest0.jpg $(IMG_PATH)/filtres0.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -sharpen 5 $(IMG_PATH)/miafilttest1.png
	diff $(IMG_PATH)/miafilttest1.png $(IMG_PATH)/filtres1.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -sharpen 5 $(IMG_PATH)/miafilttest1.jpg
	diff $(IMG_PATH)/miafilttest1.jpg $(IMG_PATH)/filtres1.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -edgedetect $(IMG_PATH)/miafilttest2.png
	diff $(IMG_PATH)/miafilttest2.png $(IMG_PATH)/filtres2.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -edgedetect $(IMG_PATH)/miafilttest2.jpg
	diff $(IMG_PATH)/miafilttest2.jpg $(IMG_PATH)/filtres2.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -thresh 0.5 $(IMG_PATH)/miafilttest3.png
	diff $(IMG_PATH)/miafilttest3.png $(IMG_PATH)/filtres3.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -thresh 0.5 $(IMG_PATH)/miafilttest3.jpg
	diff $(IMG_PATH)/miafilttest3.jpg $(IMG_PATH)/filtres3.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -saturate 1.0 $(IMG_PATH)/miafilttest4.png
	diff $(IMG_PATH)/miafilttest4.png $(IMG_PATH)/filtres4.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -saturate 1.0 $(IMG_PATH)/miafilttest4.jpg
	diff $(IMG_PATH)/miafilttest4.jpg $(IMG_PATH)/filtres4.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -multrgb 1.0,1.0,1.0 $(IMG_PATH)/miafilttest5.png
	diff $(IMG_PATH)/miafilttest5.png $(IMG_PATH)/filtres5.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -multrgb 1.0,1.0,1.0 $(IMG_PATH)/miafilttest5.jpg
	diff $(IMG_PATH)/miafilttest5.jpg $(IMG_PATH)/filtres5.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -quantize 8 $(IMG_PATH)/miafilttest6.png
	diff $(IMG_PATH)/miafilttest6.png $(IMG_PATH)/filtres6.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -quantize 8 $(IMG_PATH)/miafilttest6.jpg
	diff $(IMG_PATH)/miafilttest6.jpg $(IMG_PATH)/filtres6.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -saturate 2.0 $(IMG_PATH)/miafilttest7.png
	diff $(IMG_PATH)/miafilttest7.png $(IMG_PATH)/miafiltres7.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -saturate 2.0 $(IMG_PATH)/miafilttest7.jpg
	diff $(IMG_PATH)/miafilttest7.jpg $(IMG_PATH)/miafiltres7.jpg
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.png -multrgb 0.5,1.5,0.5 $(IMG_PATH)/miafilttest8.png
	diff $(IMG_PATH)/miafilttest8.png $(IMG_PATH)/miafiltres8.png
	$(MIA_PATH)/$(MIA) $(IMG_PATH)/base.jpg -multrgb 0.5,1.5,0.5 $(IMG_PATH)/miafilttest8.jpg
	diff $(IMG_PATH)/miafilttest8.jpg $(IMG_PATH)/miafiltres8.jpg
	@echo Mia tests completed without error
	@sleep 5


clean:
	\rm -rf $(OBJECT_DIR) $(EXECUTABLE)

$(GLUI_LIB):
	$(MAKE) -C $(GLUI_PATH) all

$(PHOTO_LIB):
	$(MAKE) -C $(PHOTO_PATH)

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
