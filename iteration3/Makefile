## A Makefile for all libraries
## and FlashPhoto + Mia

GLUI = glui
PNG = png
JPEG = jpeg
PHOTO = photo
MIA = Mia
FLASH = FlashPhoto
TEST = testex

GLUI_PATH = ./glui
JPEG_PATH = ./libjpeg
PNG_PATH = ./libpng
PHOTO_PATH = ./libphoto
MIA_PATH = ./Mia
FLASH_PATH = ./FlashPhoto
TEST_PATH = ./tests

GLUI_LIB = $(GLUI_PATH)/lib/lib$(GLUI).a
PNG_LIB = $(PNG_PATH)/lib/lib$(PNG).a
JPEG_LIB = $(JPEG_PATH)/lib/lib$(JPEG).a
PHOTO_LIB = $(PHOTO_PATH)/lib/lib$(PHOTO).a

TEST_EXEC = $(TEST_PATH)/$(TEST)
MIA_EXEC = $(MIA_PATH)/$(MIA)
FLASH_EXEC = $(FLASH_PATH)/$(FLASH)

build: $(GLUI_LIB) $(PNG_LIB) $(JPEG_LIB) $(PHOTO_LIB) $(MIA_EXEC) $(FLASH_EXEC)

all: build docs check

check: $(TEST_EXEC)
	cd $(TEST_PATH); \
	./$(TEST)

rebuild: clean build

clean: cleanall

cleanall: cleanapps cleanlibs

cleanapps: cleanmia cleanflash

cleanlibs: cleanglui cleanjpeg cleanpng cleanphoto

cleanglui:
	$(MAKE)  -C ./$(GLUI_PATH)/ clean

cleanjpeg:
	$(MAKE) -C ./$(JPEG_PATH) distclean
	cd $(JPEG_PATH); \
	rm -rf lib bin include share

cleanpng:
	$(MAKE) -C ./$(PNG_PATH) distclean
	cd $(PNG_PATH); \
	rm -rf lib bin include share

cleanphoto:
	$(MAKE) -C ./$(PHOTO_PATH) clean
	cd $(PHOTO_PATH); \
	rm -rf lib include

cleanmia:
	$(MAKE) -C ./$(MIA_PATH) clean

cleanflash:
	$(MAKE) -C ./$(FLASH_PATH) clean

$(GLUI_LIB):
	$(MAKE) -C $(GLUI_PATH) all

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
	# $(MAKE) -C $(JPEG_PATH) check
	$(MAKE) -C $(JPEG_PATH) install

$(PHOTO_LIB): $(JPEG_LIB) $(PNG_LIB)
	$(MAKE) -C $(PHOTO_PATH)

$(FLASH_EXEC): $(PHOTO_LIB)
	$(MAKE) -C $(FLASH_PATH)

$(MIA_EXEC): $(PHOTO_LIB)
	$(MAKE) -C $(MIA_PATH)

$(TEST_EXEC): $(MIA_EXEC) $(FLASH_EXEC)
	$(MAKE) -C $(TEST_PATH)

docs:
	doxygen
