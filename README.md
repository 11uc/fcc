## Introduction

This is a program used to control fear conditioning experiments. 
Intended to be used on Raspberry pi. Firstly implemented on MacOS. 
Qt and Opencv are used.

Basic functions include:
* Video recording animal behavior through camera.
* Control auditory cue 
* Control electric shock using ttl signal
* Customize experiment protocol

## Compilation

### MacOS

Compile using qmake and gnu make.
1. Create project file `qmake -project -o fcrig.pro`.
2. Add to fcrig.pro file the following lines to include Qt libaries
```
Qt += widgets
Qt += core
CONFIG += qt
```
3. Create MakeFile using `qmake -makefile fcrig.pro`.
4. In MakeFile, add `-I/path/to/opencv/library` to INPATH and add
```
-L/path/to/opencv/library/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc
```
to LIBS.
5. Build using `make`.

### Raspbian on Raspberry Pi

Compile using qmake and gnu make.
1. If QtMultimedia library not installed, install it with `sudo apt-get install qtmultimedia5-dev`.
2. Create project file `qmake -project -o fcrig.pro`.
3. Add to fcrig.pro file the following lines to include Qt libaries
```
Qt += widgets
Qt += core
CONFIG += qt
```
4. Create MakeFile using `qmake -makefile fcrig.pro`.
5. In MakeFile, 
* For opencv add `-I/path/to/opencv/library` to INPATH. add
`
-L/path/to/opencv/library/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc
` to LIBS.
* For gstreamer, add `-pthread` to CXXFLAGS, add `-I/path/to/gstreamer -I/path/to/glib` to INPATH, add `-lgstreamer-1.0 -lgobject-2.0 -lglib-2.0` to LIBS (could use `pkg-config --cflags --libs` to locate those).
6. Build using `make`.

## Use

### Video recording format.
* I found MPEG works for MacOS and XVID works for linux.
* v4l module needs to be loaded for using camera by `sudo modprobe bcm2835-v4l2`.
